/*
 * main.cpp — OuraPanel v0.2
 */

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_7c.h>
#include <Fonts/FreeSansBold12pt7b.h>

#include "dashboard_color_config.h"
#include "dashboard_layout.h"
#include "dashboard_icons.h"
#include "oura_api.h"
#include "secrets.h"

#define EPD_CS   13
#define EPD_DC   22
#define EPD_RST  25
#define EPD_BUSY 14

GxEPD2_7C<GxEPD2_730c_ACeP_730, GxEPD2_730c_ACeP_730::HEIGHT / 4> display(
  GxEPD2_730c_ACeP_730(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

static const int W = 800;
static const int H = 480;
#define BATT_PIN 34

static const char* NTP_SERVER     = "pool.ntp.org";
static const long  UTC_OFFSET_SEC = -6 * 3600;
static const int   DST_OFFSET_SEC = 3600;
static const int   ACTIVE_REFRESH_INTERVAL_MINUTES = 30;
static const int   INACTIVE_REFRESH_INTERVAL_MINUTES = 60;
static const int   REFRESH_START_HOUR = 8;
static const int   REFRESH_END_HOUR = 20;
static const int   WIFI_CONNECT_TIMEOUT_MS = 90000;
static const int   WIFI_POLL_INTERVAL_MS = 500;

static int readBatteryPct() {
  pinMode(BATT_PIN, INPUT);
  analogReadResolution(12);
  analogSetPinAttenuation(BATT_PIN, ADC_11db);
  uint32_t rawSum = 0;
  uint32_t mvSum = 0;
  const int samples = 8;
  for (int i = 0; i < samples; i++) {
    rawSum += analogRead(BATT_PIN);
    mvSum += analogReadMilliVolts(BATT_PIN);
    delay(2);
  }
  int raw = rawSum / samples;
  float pinMv = (float)(mvSum / samples);
  float voltage = (pinMv / 1000.0f) * 2.0f;
  Serial.printf("[BAT] raw=%d pin=%.0fmV vbat=%.2fV\n", raw, pinMv, voltage);
  if (voltage < 3.0f || voltage > 4.5f) return -1;
  int pct = (int)((voltage - 3.3f) / (4.2f - 3.3f) * 100.0f);
  return constrain(pct, 0, 100);
}

static String getTodayDate() {
  configTime(UTC_OFFSET_SEC, DST_OFFSET_SEC, NTP_SERVER);
  struct tm t;
  if (!getLocalTime(&t, 10000)) {
    Serial.println("[NTP] Failed to sync time");
    return "";
  }
  char buf[11];
  strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
  Serial.printf("[NTP] Today: %s\n", buf);
  return String(buf);
}

static String getDateDaysAgo(int days) {
  struct tm t;
  if (!getLocalTime(&t, 1000)) return "";
  t.tm_mday -= days;
  mktime(&t);
  char buf[11];
  strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
  return String(buf);
}

static String getTimestamp() {
  struct tm t;
  if (!getLocalTime(&t, 1000)) return "--:--";
  int h = t.tm_hour;
  bool pm = h >= 12;
  if (h > 12) h -= 12;
  if (h == 0) h = 12;
  char buf[12];
  snprintf(buf, sizeof(buf), "%d:%02d%s", h, t.tm_min, pm ? "pm" : "am");
  return String(buf);
}

static bool isWithinRefreshWindow(const struct tm& t) {
  return t.tm_hour >= REFRESH_START_HOUR && t.tm_hour < REFRESH_END_HOUR;
}

static int refreshIntervalMinutesFor(const struct tm& t) {
  return isWithinRefreshWindow(t) ? ACTIVE_REFRESH_INTERVAL_MINUTES : INACTIVE_REFRESH_INTERVAL_MINUTES;
}

static void drawCenteredText(const String& text, const GFXfont* font, uint16_t color, int y) {
  display.setFont(font);
  int16_t x1, y1; uint16_t tw, th;
  display.getTextBounds(text, 0, 0, &x1, &y1, &tw, &th);
  display.setTextColor(color);
  display.setCursor((W - (int)tw) / 2, y);
  display.print(text);
}

static void showStatusScreen(const String& title, const String& subtitle, uint16_t titleColor) {
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    drawCenteredText(title, &FreeSansBold12pt7b, titleColor, H / 2 - 12);
    drawCenteredText(subtitle, &FreeSansBold12pt7b, GxEPD_BLACK, H / 2 + 22);
  } while (display.nextPage());
}

static void showWifiErrorScreen(const String& ssid) {
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    drawIconWifiOff(W / 2, H / 2 - 88, 64, DashboardColorConfig::RED);
    drawCenteredText("WiFi failed", &FreeSansBold12pt7b, DashboardColorConfig::RED, H / 2 + 8);
    drawCenteredText(ssid, &FreeSansBold12pt7b, GxEPD_BLACK, H / 2 + 42);
    drawCenteredText("Check SSID or signal", &FreeSansBold12pt7b, DashboardColorConfig::DARKGREY, H / 2 + 76);
  } while (display.nextPage());
}

static const char* wifiStatusLabel(wl_status_t status) {
  switch (status) {
    case WL_IDLE_STATUS: return "IDLE";
    case WL_NO_SSID_AVAIL: return "NO_SSID";
    case WL_SCAN_COMPLETED: return "SCAN_DONE";
    case WL_CONNECTED: return "CONNECTED";
    case WL_CONNECT_FAILED: return "CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "CONNECTION_LOST";
    case WL_DISCONNECTED: return "DISCONNECTED";
    default: return "UNKNOWN";
  }
}

static String authFailureActionMessage(const String& status) {
  if (status.indexOf("No refresh token") >= 0 ||
      status.indexOf("Bootstrap refresh token is empty") >= 0 ||
      status.indexOf("Refresh skipped: no refresh token") >= 0) {
    return "Add token in secrets.cpp";
  }

  if (status.indexOf("HTTP 400") >= 0 || status.indexOf("HTTP 401") >= 0) {
    return "Run authorize.py again";
  }

  if (status.indexOf("NVS") >= 0) {
    return "Clear NVS, then reboot";
  }

  return "Open serial for auth cause";
}

static bool tryWifiConnect(const char* phase, bool eraseStoredAp) {
  Serial.printf("[WIFI] %s: connecting to %s\n", phase, WIFI_SSID);

  if (eraseStoredAp) {
    Serial.println("[WIFI] Clearing stored AP credentials from NVS");
    WiFi.disconnect(true, true);
    delay(300);
  } else {
    WiFi.disconnect(true);
    delay(100);
  }

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.persistent(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  uint32_t start = millis();
  wl_status_t lastStatus = WiFi.status();
  Serial.printf("[WIFI] Initial status=%s (%d)\n", wifiStatusLabel(lastStatus), (int)lastStatus);

  while (lastStatus != WL_CONNECTED && (millis() - start) < WIFI_CONNECT_TIMEOUT_MS) {
    delay(WIFI_POLL_INTERVAL_MS);
    wl_status_t status = WiFi.status();
    Serial.print(".");
    if (status != lastStatus) {
      Serial.printf("\n[WIFI] Status=%s (%d)\n", wifiStatusLabel(status), (int)status);
      lastStatus = status;
    }
  }
  Serial.println();

  wl_status_t finalStatus = WiFi.status();
  if (finalStatus == WL_CONNECTED) {
    Serial.printf("[WIFI] Connected — %s\n", WiFi.localIP().toString().c_str());
    return true;
  }

  Serial.printf("[WIFI] %s failed after %lu ms. Final status=%s (%d)\n",
                phase, (unsigned long)(millis() - start), wifiStatusLabel(finalStatus), (int)finalStatus);
  return false;
}

static void enterDeepSleep() {
  struct tm t;
  uint64_t sleepUs = (uint64_t)INACTIVE_REFRESH_INTERVAL_MINUTES * 60ULL * 1000000ULL;
  if (getLocalTime(&t, 1000)) {
    int intervalMinutes = refreshIntervalMinutesFor(t);
    int secondsPastBoundary = (t.tm_min % intervalMinutes) * 60 + t.tm_sec;
    int sleepSeconds = intervalMinutes * 60 - secondsPastBoundary;
    if (sleepSeconds <= 0) {
      sleepSeconds = intervalMinutes * 60;
    }

    Serial.printf("[SLEEP] %s refresh interval: sleeping until next %d-minute mark in %d seconds...\n",
                  isWithinRefreshWindow(t) ? "Active-window" : "Off-hours",
                  intervalMinutes,
                  sleepSeconds);
    sleepUs = (uint64_t)sleepSeconds * 1000000ULL;
  } else {
    Serial.printf("[SLEEP] Time unavailable, sleeping for %d minutes...\n", INACTIVE_REFRESH_INTERVAL_MINUTES);
  }
  Serial.flush();
  esp_deep_sleep(sleepUs);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  display.init(115200, true, 50, false);
  display.setRotation(0);

  int battPct = readBatteryPct();
  Serial.printf("[BAT] Battery: %d%%\n", battPct);

  if (!tryWifiConnect("Attempt 1", false) && !tryWifiConnect("Attempt 2 (forced clear)", true)) {
    Serial.println("[WIFI] Failed - showing error screen and sleeping");
    showWifiErrorScreen(String(WIFI_SSID));
    display.hibernate();
    WiFi.disconnect(true);
    enterDeepSleep();
  }

  String today = getTodayDate();

  String trendStartDate = getDateDaysAgo(15);

  loadTokens();
  if (usingBootstrapRefreshToken()) {
    Serial.println("[AUTH] Warning: using bootstrap token from secrets; NVS token missing");
  }
  if (!refreshAccessToken()) {
    Serial.println("[AUTH] Refresh failed");
    const String authStatus = getAuthStatusMessage();
    Serial.printf("[AUTH] Status: %s\n", authStatus.c_str());
    Serial.println("[AUTH] Re-authorize only if NVS is empty or the token chain was revoked.");
    showStatusScreen("Auth failed", authFailureActionMessage(authStatus), GxEPD_RED);
    display.hibernate();
    enterDeepSleep();
  }

  DashboardData data = fetchDashboardData(today, trendStartDate);
  data.lastUpdated = getTimestamp();
  drawDashboardLayout(data, battPct);
  display.hibernate();
  WiFi.disconnect(true);
  enterDeepSleep();
}

void loop() {}
