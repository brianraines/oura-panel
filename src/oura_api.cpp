#include "oura_api.h"

#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <time.h>

#include "secrets.h"

static const char* OURA_TOKEN_URL = "https://api.ouraring.com/oauth/token";
static const char* OURA_API_BASE  = "https://api.ouraring.com";
static const int HTTP_CONNECT_TIMEOUT_MS = 15000;
static const int HTTP_READ_TIMEOUT_MS    = 15000;
static const int HTTP_TRANSIENT_RETRIES  = 1;
static const int HTTP_TRANSIENT_BACKOFF_MS = 500;
static const int HR_ONE_HOUR_BINS = 60; // 1-minute buckets across the past hour

static Preferences prefs;
static String accessToken;
static String refreshToken;
static bool refreshTokenFromNvs = false;
static long hrBinSumBuf[MAX_HR_PLOT_POINTS];
static int hrBinCntBuf[MAX_HR_PLOT_POINTS];
static int hrBinMinBuf[MAX_HR_PLOT_POINTS];
static int hrBinMaxBuf[MAX_HR_PLOT_POINTS];

static bool isTransientHttpClientError(int code) {
  return code == HTTPC_ERROR_READ_TIMEOUT ||        // -11
         code == HTTPC_ERROR_CONNECTION_LOST ||     // -5
         code == HTTPC_ERROR_CONNECTION_REFUSED ||  // -1
         code == HTTPC_ERROR_NOT_CONNECTED;         // -4
}

static String nextDate(const String& yyyyMmDd) {
  int y = 0, m = 0, d = 0;
  if (sscanf(yyyyMmDd.c_str(), "%d-%d-%d", &y, &m, &d) != 3) return yyyyMmDd;

  struct tm t = {};
  t.tm_year = y - 1900;
  t.tm_mon  = m - 1;
  t.tm_mday = d + 1;
  if (mktime(&t) < 0) return yyyyMmDd;

  char buf[11];
  strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
  return String(buf);
}

static String prevDate(const String& yyyyMmDd) {
  int y = 0, m = 0, d = 0;
  if (sscanf(yyyyMmDd.c_str(), "%d-%d-%d", &y, &m, &d) != 3) return yyyyMmDd;

  struct tm t = {};
  t.tm_year = y - 1900;
  t.tm_mon  = m - 1;
  t.tm_mday = d - 1;
  if (mktime(&t) < 0) return yyyyMmDd;

  char buf[11];
  strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
  return String(buf);
}

static bool dateMatchesToday(const String& s, const String& today) {
  if (s.length() < 10) return false;
  return s.substring(0, 10) == today;
}

static int findTodayIndex(JsonArray arr, const String& today) {
  int match = -1;
  for (int i = 0; i < (int)arr.size(); i++) {
    JsonObject o = arr[i];
    if (dateMatchesToday(o["day"].as<String>(), today) ||
        dateMatchesToday(o["date"].as<String>(), today) ||
        dateMatchesToday(o["start_datetime"].as<String>(), today) ||
        dateMatchesToday(o["timestamp"].as<String>(), today)) {
      match = i; // keep latest match in array order
    }
  }
  return match;
}

static int findTrendIndexByDay(const DashboardData& d, const String& day) {
  for (int i = 0; i < d.trendCount; i++) {
    if (d.trends[i].day == day) return i;
  }
  return -1;
}

static int64_t daysFromCivil(int y, unsigned m, unsigned d) {
  y -= m <= 2;
  const int era = (y >= 0 ? y : y - 399) / 400;
  const unsigned yoe = (unsigned)(y - era * 400);                      // [0, 399]
  const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; // [0, 365]
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;           // [0, 146096]
  return era * 146097 + (int64_t)doe - 719468;                           // Days since 1970-01-01
}

static time_t parseIsoTimestampToEpoch(const String& ts) {
  int y = 0, m = 0, d = 0, hh = 0, mm = 0, ss = 0;
  // Handles formats like 2026-03-03T16:55:00.500Z
  if (sscanf(ts.c_str(), "%d-%d-%dT%d:%d:%d", &y, &m, &d, &hh, &mm, &ss) < 6) {
    return (time_t)-1;
  }
  int64_t days = daysFromCivil(y, (unsigned)m, (unsigned)d);
  int64_t epoch = days * 86400LL + (int64_t)hh * 3600LL + (int64_t)mm * 60LL + (int64_t)ss;
  return (time_t)epoch; // UTC epoch (timestamps are Zulu time)
}

// ---------------------------------------------------------------------------
// Token persistence (ESP32 NVS)
// ---------------------------------------------------------------------------

static void saveTokens() {
  prefs.begin("oura", false);
  prefs.putString("access", accessToken);
  prefs.putString("refresh", refreshToken);
  prefs.end();
}

bool loadTokens() {
  accessToken = "";
  refreshToken = "";
  refreshTokenFromNvs = false;

  if (prefs.begin("oura", true)) {
    refreshToken = prefs.getString("refresh", "");
    prefs.end();
  }

  if (refreshToken.length() > 0) {
    refreshTokenFromNvs = true;
    Serial.println("[AUTH] Using refresh token from NVS");
  } else {
    refreshToken = OURA_REFRESH_TOKEN;
    Serial.println("[AUTH] Using seed refresh token from secrets.h");
  }
  Serial.printf("[AUTH] Refresh token: %s\n", refreshToken.c_str());
  return refreshToken.length() > 0;
}

bool hasAccessToken() {
  return accessToken.length() > 0;
}

bool useSeedRefreshToken() {
  accessToken = "";
  refreshToken = OURA_REFRESH_TOKEN;
  refreshTokenFromNvs = false;

  if (refreshToken.length() == 0) {
    Serial.println("[AUTH] Seed refresh token is empty");
    return false;
  }

  Serial.println("[AUTH] Falling back to seed refresh token from secrets.h");
  Serial.printf("[AUTH] Refresh token: %s\n", refreshToken.c_str());
  return true;
}



// ---------------------------------------------------------------------------
// OAuth2: refresh the access token
// ---------------------------------------------------------------------------

bool refreshAccessToken() {
  if (refreshToken.length() == 0) return false;

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.begin(client, OURA_TOKEN_URL);
  http.setConnectTimeout(HTTP_CONNECT_TIMEOUT_MS);
  http.setTimeout(HTTP_READ_TIMEOUT_MS);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String body = "grant_type=refresh_token";
  body += "&refresh_token=" + refreshToken;
  body += "&client_id="     + String(OURA_CLIENT_ID);
  body += "&client_secret=" + String(OURA_CLIENT_SECRET);

  Serial.printf("[AUTH] Refreshing access token (%s)...\n",
                refreshTokenFromNvs ? "NVS" : "seed");
  int httpCode = http.POST(body);

  if (httpCode == 200) {
    JsonDocument doc;
    deserializeJson(doc, http.getString());
    accessToken  = doc["access_token"].as<String>();
    refreshToken = doc["refresh_token"].as<String>();
    Serial.printf("[AUTH] OK (expires in %d s)\n", doc["expires_in"] | 0);
    saveTokens();
    http.end();
    return true;
  }

  Serial.printf("[AUTH] Failed (HTTP %d): %s\n", httpCode, http.getString().c_str());
  http.end();
  return false;
}

// ---------------------------------------------------------------------------
// Authenticated GET helper
// ---------------------------------------------------------------------------

String apiGet(const String& path) {
  String url = String(OURA_API_BASE) + path;
  bool refreshedToken = false;
  int transientRetryCount = 0;

  while (true) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.begin(client, url);
    http.setConnectTimeout(HTTP_CONNECT_TIMEOUT_MS);
    http.setTimeout(HTTP_READ_TIMEOUT_MS);
    http.addHeader("Authorization", "Bearer " + accessToken);

    int code = http.GET();
    String payload = http.getString();
    http.end();

    if (code == 401 && !refreshedToken) {
      // Serial.printf("[API] %s → 401: %s\n", path.c_str(), payload.c_str());
      if (!refreshAccessToken()) break;
      Serial.printf("[API] %s → retrying with new token...\n", path.c_str());
      refreshedToken = true;
      continue;
    }

    if (code < 0 && isTransientHttpClientError(code) &&
        transientRetryCount < HTTP_TRANSIENT_RETRIES) {
      transientRetryCount++;
      Serial.printf("[API] %s → transient client error %d, retrying (%d/%d)\n",
                    path.c_str(), code, transientRetryCount, HTTP_TRANSIENT_RETRIES);
      delay(HTTP_TRANSIENT_BACKOFF_MS);
      continue;
    }

    if (code != 200) {
      // Serial.printf("[API] %s → HTTP %d: %s\n", path.c_str(), code, payload.c_str());
      return "";
    }

    Serial.printf("[API] %s → 200\n", path.c_str());
    return payload;
  }

  Serial.printf("[API] %s → failed after token refresh\n", path.c_str());
  return "";
}

// ---------------------------------------------------------------------------
// Endpoint fetchers
// ---------------------------------------------------------------------------

static void fetchActivity(DashboardData& d, const String& today) {
  String startDate = prevDate(today);
  String endDate = nextDate(today);
  String payload = apiGet("/v2/usercollection/daily_activity?start_date=" + startDate + "&end_date=" + endDate);

  // Serial.printf("[API] Activity payload: %s\n", payload.c_str());

  if (!payload.length()) return;

  JsonDocument doc;
  if (deserializeJson(doc, payload)) return;
  JsonArray arr = doc["data"].as<JsonArray>();
  if (!arr.size()) return;

  int idx = findTodayIndex(arr, today);
  if (idx < 0) return;
  JsonObject o = arr[idx];
  String activityJson;
  serializeJson(o, activityJson);
  //Serial.printf("[DATA] Activity: %s\n", activityJson.c_str());
  d.activityScore = o["score"] | -1;
  d.steps = o["steps"] | -1;
  Serial.printf("[DATA] Activity score=%d steps=%d\n", d.activityScore, d.steps);
}

static void fetchCardioAge(DashboardData& d, const String& today) {
  String startDate = prevDate(today);
  String endDate = nextDate(today);
  String payload = apiGet("/v2/usercollection/daily_cardiovascular_age?start_date=" + startDate + "&end_date=" + endDate);
  if (!payload.length()) return;

  JsonDocument doc;
  if (deserializeJson(doc, payload)) return;
  JsonArray arr = doc["data"].as<JsonArray>();
  if (!arr.size()) return;

  int idx = findTodayIndex(arr, today);
  if (idx < 0) return;
  d.vascularAge = arr[idx]["vascular_age"] | -1;
  Serial.printf("[DATA] Vascular age=%d\n", d.vascularAge);
}

static void fetchReadiness(DashboardData& d, const String& today) {
  String startDate = prevDate(today);
  String endDate = nextDate(today);
  String payload = apiGet("/v2/usercollection/daily_readiness?start_date=" + startDate + "&end_date=" + endDate);

  // Serial.printf("[API] Readiness payload: %s\n", payload.c_str());
  if (!payload.length()) return;

  JsonDocument doc;
  if (deserializeJson(doc, payload)) return;
  JsonArray arr = doc["data"].as<JsonArray>();
  if (!arr.size()) return;

  int idx = findTodayIndex(arr, today);
  if (idx < 0) return;
  JsonObject o = arr[idx];
  d.readinessScore = o["score"] | -1;
  JsonObject c = o["contributors"];
  d.hrvBalance = c["hrv_balance"] | -1;
  d.restingHeartRate = c["resting_heart_rate"] | -1;
  Serial.printf("[DATA] Readiness score=%d hrvBal=%d rhr=%d\n",
                d.readinessScore, d.hrvBalance, d.restingHeartRate);
}

static void fetchResilience(DashboardData& d, const String& today) {
  String startDate = prevDate(today);
  String endDate = nextDate(today);
  String payload = apiGet("/v2/usercollection/daily_resilience?start_date=" + startDate + "&end_date=" + endDate);

  // Serial.printf("[API] Resilience payload: %s\n", payload.c_str());
  if (!payload.length()) return;

  JsonDocument doc;
  if (deserializeJson(doc, payload)) return;
  JsonArray arr = doc["data"].as<JsonArray>();
  if (!arr.size()) return;

  int idx = findTodayIndex(arr, today);
  if (idx < 0) return;
  JsonObject o = arr[idx];
  d.resilienceLevel = o["level"].as<String>();
  JsonObject c = o["contributors"];
  if (!c["stress"].isNull()) {
    d.stress = (int)(c["stress"].as<float>() + 0.5f);
  } else {
    d.stress = -1;
  }
  Serial.printf("[DATA] Resilience level=%s stress=%d\n",
                d.resilienceLevel.c_str(), d.stress);
}

static void fetchDailySleep(DashboardData& d, const String& tenDaysAgo, const String& today) {
  String endDate = nextDate(today);
  String payload = apiGet("/v2/usercollection/daily_sleep?start_date=" + tenDaysAgo + "&end_date=" + endDate);
  if (!payload.length()) return;

  JsonDocument doc;
  if (deserializeJson(doc, payload)) return;
  JsonArray arr = doc["data"].as<JsonArray>();
  if (!arr.size()) return;

  int todayIdx = findTodayIndex(arr, today);
  if (todayIdx >= 0) d.sleepScore = arr[todayIdx]["score"] | -1;
  else d.sleepScore = arr[arr.size() - 1]["score"] | -1;

  int start = ((int)arr.size() > MAX_TREND_DAYS) ? (int)arr.size() - MAX_TREND_DAYS : 0;
  d.trendCount = min((int)arr.size(), MAX_TREND_DAYS);
  for (int i = 0; i < d.trendCount; i++) {
    d.trends[i].day = arr[start + i]["day"].as<String>();
    d.trends[i].score = arr[start + i]["score"] | -1;
  }
  Serial.printf("[DATA] Daily sleep: today=%d, %d trend entries\n",
                d.sleepScore, d.trendCount);
}

static void fetchSleepDetail(DashboardData& d, const String& tenDaysAgo, const String& today) {
  String endDate = nextDate(today);
  String payload = apiGet("/v2/usercollection/sleep?start_date=" + tenDaysAgo + "&end_date=" + endDate);
  if (!payload.length()) return;

  JsonDocument doc;
  if (deserializeJson(doc, payload)) return;
  JsonArray arr = doc["data"].as<JsonArray>();
  if (!arr.size()) return;

  // Find the latest long_sleep entry for detail metrics
  int latestIdx = -1;
  for (int i = (int)arr.size() - 1; i >= 0; i--) {
    String type = arr[i]["type"].as<String>();
    if (type == "long_sleep" || type.length() == 0) {
      latestIdx = i;
      break;
    }
  }
  if (latestIdx < 0) latestIdx = (int)arr.size() - 1;

  JsonObject latest = arr[latestIdx];
  d.sleepDay        = latest["day"].as<String>();
  d.avgBreath       = latest["average_breath"] | -1.0f;
  d.avgHeartRate    = latest["average_heart_rate"] | -1;
  d.lowestHeartRate = latest["lowest_heart_rate"] | -1;
  d.avgHrv          = latest["average_hrv"] | -1;
  d.efficiency      = latest["efficiency"] | -1;
  d.timeInBed       = latest["time_in_bed"] | -1;
  d.awakeTime       = latest["awake_time"] | -1;
  d.totalSleep      = latest["total_sleep_duration"] | -1;
  d.lightSleep      = latest["light_sleep_duration"] | -1;
  d.remSleep        = latest["rem_sleep_duration"] | -1;
  d.deepSleep       = latest["deep_sleep_duration"] | -1;
  d.bedtimeStart    = latest["bedtime_start"].as<String>();
  d.bedtimeEnd      = latest["bedtime_end"].as<String>();

  // Peak HRV: highest non-null value in hrv.items
  if (!latest["hrv"].isNull()) {
    JsonArray items = latest["hrv"]["items"].as<JsonArray>();
    int maxVal = -1;
    for (JsonVariant v : items) {
      if (!v.isNull()) {
        int val = (int)(v.as<float>() + 0.5f);
        if (val > maxVal) maxVal = val;
      }
    }
    d.peakHrv = maxVal;
  }

  // Build per-day duration data for trends (match by day, do not assume ordering)
  String lastDay;
  for (int i = (int)arr.size() - 1; i >= 0; i--) {
    String type = arr[i]["type"].as<String>();
    if (type.length() > 0 && type != "long_sleep") continue;

    String day = arr[i]["day"].as<String>();
    if (day == lastDay) continue;
    lastDay = day;

    int trendIdx = findTrendIndexByDay(d, day);
    if (trendIdx < 0) continue;

    d.trends[trendIdx].timeInBed  = arr[i]["time_in_bed"] | -1;
    d.trends[trendIdx].awakeTime  = arr[i]["awake_time"] | -1;
    d.trends[trendIdx].totalSleep = arr[i]["total_sleep_duration"] | -1;
    d.trends[trendIdx].lightSleep = arr[i]["light_sleep_duration"] | -1;
    d.trends[trendIdx].remSleep   = arr[i]["rem_sleep_duration"] | -1;
    d.trends[trendIdx].deepSleep  = arr[i]["deep_sleep_duration"] | -1;
  }

  Serial.printf("[DATA] Sleep detail: day=%s HR=%d HRV=%d peakHRV=%d eff=%d\n",
                d.sleepDay.c_str(), d.avgHeartRate, d.avgHrv, d.peakHrv, d.efficiency);
}

static void fetchHeartRate(DashboardData& d, const String& today) {
  String startDate = prevDate(today);
  String endDate = nextDate(today);
  String payload = apiGet("/v2/usercollection/heartrate?start_date=" + startDate + "&end_date=" + endDate);
  if (!payload.length()) return;

  JsonDocument doc;
  if (deserializeJson(doc, payload)) return;
  JsonArray arr = doc["data"].as<JsonArray>();
  if (!arr.size()) return;

  long sum = 0;
  int count = 0;
  int minHr = 999;
  String latestTs = "";
  int latestHr = -1;
  int latestHrv = -1;
  int totalValidSamples = 0;
  time_t latestEpoch = (time_t)-1;

  for (JsonVariant v : arr) {
    JsonObject o = v.as<JsonObject>();
    String ts = o["timestamp"].as<String>();
    if (ts.length() < 10) continue;
    int hr = o["bpm"] | -1;
    if (hr < 0) hr = o["heart_rate"] | -1;
    if (hr < 0) hr = o["value"] | -1;
    if (hr < 0) continue;
    totalValidSamples++;

    int hrv = -1;
    if (!o["hrv"].isNull()) {
      hrv = (int)(o["hrv"].as<float>() + 0.5f);
    } else if (!o["rmssd"].isNull()) {
      hrv = (int)(o["rmssd"].as<float>() + 0.5f);
    }

    sum += hr;
    count++;
    if (hr < minHr) minHr = hr;

    if (latestTs.length() == 0 || ts > latestTs) {
      latestTs = ts;
      latestHr = hr;
      latestHrv = hrv;
    }
    time_t epoch = parseIsoTimestampToEpoch(ts);
    if (epoch != (time_t)-1 && (latestEpoch == (time_t)-1 || epoch > latestEpoch)) {
      latestEpoch = epoch;
    }
  }

  if (count > 0) {
    d.avgHeartRate = (int)(sum / count);
    d.lowestHeartRate = minHr;
    d.currentHeartRate = latestHr;
    d.currentHrv = latestHrv;
    Serial.printf("[DATA] Heartrate avg=%d low=%d current=%d currentHrv=%d samples=%d\n",
                  d.avgHeartRate, d.lowestHeartRate, d.currentHeartRate, d.currentHrv, count);
  }

  // Build dense plot samples from raw heartrate series using min/max envelopes
  // in fixed time bins over the last hour (X: BPM, Y: time).
  d.hrPlotCount = 0;
  if (totalValidSamples > 0 && latestEpoch != (time_t)-1) {
    const time_t oneHourAgo = latestEpoch - 3600;
    d.hrPlotWindowStart = oneHourAgo;
    d.hrPlotWindowEnd = latestEpoch;
    int bins = min(HR_ONE_HOUR_BINS, MAX_HR_PLOT_POINTS);
    memset(hrBinSumBuf, 0, sizeof(hrBinSumBuf));
    memset(hrBinCntBuf, 0, sizeof(hrBinCntBuf));
    for (int i = 0; i < bins; i++) {
      hrBinMinBuf[i] = 999;
      hrBinMaxBuf[i] = -1;
    }
    int hourValidSamples = 0;
    for (JsonVariant v : arr) {
      JsonObject o = v.as<JsonObject>();
      int hr = o["bpm"] | -1;
      if (hr < 0) hr = o["heart_rate"] | -1;
      if (hr < 0) hr = o["value"] | -1;
      if (hr < 0) continue;

      time_t epoch = parseIsoTimestampToEpoch(o["timestamp"].as<String>());
      if (epoch == (time_t)-1) continue;
      if (epoch < oneHourAgo || epoch > latestEpoch) continue;
      hourValidSamples++;

      long secSinceStart = (long)(epoch - oneHourAgo);
      int b = (int)(secSinceStart * bins / 3600L);
      if (b >= bins) b = bins - 1;
      hrBinSumBuf[b] += hr;
      hrBinCntBuf[b]++;
      if (hr < hrBinMinBuf[b]) hrBinMinBuf[b] = hr;
      if (hr > hrBinMaxBuf[b]) hrBinMaxBuf[b] = hr;
    }

    if (hourValidSamples <= 0) {
      // Fallback: if 1-hour window unexpectedly has zero parsable samples,
      // fill bins from all valid samples so the chart is never empty.
      memset(hrBinSumBuf, 0, sizeof(hrBinSumBuf));
      memset(hrBinCntBuf, 0, sizeof(hrBinCntBuf));
      for (int i = 0; i < bins; i++) {
        hrBinMinBuf[i] = 999;
        hrBinMaxBuf[i] = -1;
      }
      int idxAll = 0;
      for (JsonVariant v : arr) {
        JsonObject o = v.as<JsonObject>();
        int hr = o["bpm"] | -1;
        if (hr < 0) hr = o["heart_rate"] | -1;
        if (hr < 0) hr = o["value"] | -1;
        if (hr < 0) continue;

        int b = (int)((long)idxAll * bins / max(1, totalValidSamples));
        if (b >= bins) b = bins - 1;
        hrBinSumBuf[b] += hr;
        hrBinCntBuf[b]++;
        if (hr < hrBinMinBuf[b]) hrBinMinBuf[b] = hr;
        if (hr > hrBinMaxBuf[b]) hrBinMaxBuf[b] = hr;
        idxAll++;
      }
      hourValidSamples = totalValidSamples;
    }
    d.hrPlotCount = bins;
    for (int i = 0; i < bins; i++) {
      if (hrBinCntBuf[i] > 0) {
        d.hrPlotBpm[i] = (int)(hrBinSumBuf[i] / hrBinCntBuf[i]);
        d.hrPlotMin[i] = hrBinMinBuf[i];
        d.hrPlotMax[i] = hrBinMaxBuf[i];
      } else {
        d.hrPlotBpm[i] = -1;
        d.hrPlotMin[i] = -1;
        d.hrPlotMax[i] = -1;
      }
    }
  }

}

// ---------------------------------------------------------------------------
// Averages
// ---------------------------------------------------------------------------

static void computeAverages(DashboardData& d) {
  int nScore = 0, nDur = 0;
  long sumScore = 0, sumTIB = 0, sumAwake = 0, sumTotal = 0;
  long sumLight = 0, sumREM = 0, sumDeep = 0;

  for (int i = 0; i < d.trendCount; i++) {
    SleepTrend& t = d.trends[i];
    if (t.score >= 0) { sumScore += t.score; nScore++; }
    if (t.totalSleep >= 0) {
      nDur++;
      sumTIB   += max(0, t.timeInBed);
      sumAwake += max(0, t.awakeTime);
      sumTotal += t.totalSleep;
      sumLight += max(0, t.lightSleep);
      sumREM   += max(0, t.remSleep);
      sumDeep  += max(0, t.deepSleep);
    }
  }

  if (nScore > 0) d.avgSleepScore = (int)(sumScore / nScore);
  if (nDur > 0) {
    d.avgTimeInBed  = (int)(sumTIB   / nDur);
    d.avgAwakeTime  = (int)(sumAwake / nDur);
    d.avgTotalSleep = (int)(sumTotal / nDur);
    d.avgLightSleep = (int)(sumLight / nDur);
    d.avgRemSleep   = (int)(sumREM   / nDur);
    d.avgDeepSleep  = (int)(sumDeep  / nDur);
  }
}

// ---------------------------------------------------------------------------
// Orchestrator
// ---------------------------------------------------------------------------

DashboardData fetchDashboardData(const String& today, const String& tenDaysAgo) {
  DashboardData d;
  d.date = today;

  fetchActivity(d, today);
  fetchCardioAge(d, today);
  fetchReadiness(d, today);
  fetchResilience(d, today);
  fetchDailySleep(d, tenDaysAgo, today);
  fetchSleepDetail(d, tenDaysAgo, today);
  fetchHeartRate(d, today);
  computeAverages(d);

  return d;
}
