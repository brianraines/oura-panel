/*
 * main.cpp — OuraPanel v0.2
 */

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_7c.h>
#include <Fonts/FreeSansBold12pt7b.h>

#include "dashboard_layout.h"
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
static const int   SLEEP_INTERVAL_MINUTES = 10;
static const int   REFRESH_START_HOUR = 8;
static const int   REFRESH_END_HOUR = 20;

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

static int secondsUntilNextRefreshWindow(const struct tm& now) {
  struct tm next = now;
  next.tm_sec = 0;

  if (now.tm_hour < REFRESH_START_HOUR) {
    next.tm_hour = REFRESH_START_HOUR;
    next.tm_min = 0;
  } else {
    next.tm_mday += 1;
    next.tm_hour = REFRESH_START_HOUR;
    next.tm_min = 0;
  }

  time_t nowEpoch = mktime(const_cast<struct tm*>(&now));
  time_t nextEpoch = mktime(&next);
  if (nowEpoch < 0 || nextEpoch <= nowEpoch) {
    return SLEEP_INTERVAL_MINUTES * 60;
  }
  return (int)(nextEpoch - nowEpoch);
}

static void drawCenteredText(const String& text, const GFXfont* font, uint16_t color, int y) {
  display.setFont(font);
  int16_t x1, y1; uint16_t tw, th;
  display.getTextBounds(text, 0, 0, &x1, &y1, &tw, &th);
  display.setTextColor(color);
  display.setCursor((W - (int)tw) / 2, y);
  display.print(text);
}

static void enterDeepSleep() {
  struct tm t;
  uint64_t sleepUs = (uint64_t)SLEEP_INTERVAL_MINUTES * 60ULL * 1000000ULL;
  if (getLocalTime(&t, 1000)) {
    int sleepSeconds = 0;
    if (isWithinRefreshWindow(t)) {
      int secondsPastBoundary = (t.tm_min % SLEEP_INTERVAL_MINUTES) * 60 + t.tm_sec;
      int secondsUntilNextBoundary = SLEEP_INTERVAL_MINUTES * 60 - secondsPastBoundary;
      if (secondsUntilNextBoundary <= 0) {
        secondsUntilNextBoundary = SLEEP_INTERVAL_MINUTES * 60;
      }

      int nextMinute = t.tm_min + (secondsUntilNextBoundary / 60);
      int nextHour = t.tm_hour + (nextMinute / 60);
      if (nextHour >= REFRESH_END_HOUR) {
        sleepSeconds = secondsUntilNextRefreshWindow(t);
        Serial.printf("[SLEEP] Outside refresh window after this cycle, sleeping until %02d:00 in %d seconds...\n",
                      REFRESH_START_HOUR, sleepSeconds);
      } else {
        sleepSeconds = secondsUntilNextBoundary;
        Serial.printf("[SLEEP] Sleeping until next %d-minute mark in %d seconds...\n", SLEEP_INTERVAL_MINUTES,
                      sleepSeconds);
      }
    } else {
      sleepSeconds = secondsUntilNextRefreshWindow(t);
      Serial.printf("[SLEEP] Outside refresh window, sleeping until %02d:00 in %d seconds...\n",
                    REFRESH_START_HOUR, sleepSeconds);
    }
    sleepUs = (uint64_t)sleepSeconds * 1000000ULL;
  } else {
    Serial.printf("[SLEEP] Time unavailable, sleeping for %d minutes...\n", SLEEP_INTERVAL_MINUTES);
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

  Serial.printf("[WIFI] Connecting to %s", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (++retries > 40) {
      Serial.println("\n[WIFI] Failed — restarting");
      ESP.restart();
    }
  }
  Serial.println();
  Serial.printf("[WIFI] Connected — %s\n", WiFi.localIP().toString().c_str());

  String today = getTodayDate();
  struct tm now;
  if (!getLocalTime(&now, 1000) || !isWithinRefreshWindow(now)) {
    Serial.printf("[SCHED] Outside refresh window (%02d:00-%02d:00); skipping refresh.\n",
                  REFRESH_START_HOUR, REFRESH_END_HOUR);
    display.hibernate();
    WiFi.disconnect(true);
    enterDeepSleep();
  }

  String trendStartDate = getDateDaysAgo(15);

  loadTokens();
  if (!refreshAccessToken()) {
    Serial.println("[AUTH] Primary token refresh failed");
    if (!(useSeedRefreshToken() && refreshAccessToken())) {
      Serial.println("[AUTH] Failed — check secrets.h / run scripts/authorize.py");
      display.setFullWindow();
      display.firstPage();
      do {
        display.fillScreen(GxEPD_WHITE);
        drawCenteredText("Auth failed - check serial", &FreeSansBold12pt7b, GxEPD_RED, H / 2);
      } while (display.nextPage());
      display.hibernate();
      enterDeepSleep();
    }
  }

  DashboardData data = fetchDashboardData(today, trendStartDate);
  data.lastUpdated = getTimestamp();
  drawDashboardLayout(data, battPct);
  display.hibernate();
  WiFi.disconnect(true);
  enterDeepSleep();
}

void loop() {}
