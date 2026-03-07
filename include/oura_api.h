#ifndef OURA_API_H
#define OURA_API_H

#include <Arduino.h>

#define MAX_TREND_DAYS 15
#define MAX_HR_PLOT_POINTS 120

struct SleepTrend {
  String day;
  int score      = -1;
  int timeInBed  = -1;
  int awakeTime  = -1;
  int totalSleep = -1;
  int lightSleep = -1;
  int remSleep   = -1;
  int deepSleep  = -1;
};

struct DashboardData {
  // Daily Activity
  int activityScore = -1;
  int steps         = -1;

  // Daily Cardiovascular Age
  int vascularAge = -1;

  // Daily Readiness
  int readinessScore   = -1;
  int hrvBalance       = -1;
  int restingHeartRate = -1;

  // Daily Resilience
  String resilienceLevel;
  int stress = -1;

  // Sleep (latest night)
  int sleepScore      = -1;
  float avgBreath     = -1;
  int avgHeartRate    = -1;
  int lowestHeartRate = -1;
  int avgHrv          = -1;
  int currentHeartRate = -1;
  int currentHrv       = -1;
  int peakHrv         = -1;
  int hrPlotBpm[MAX_HR_PLOT_POINTS] = {};
  int hrPlotMin[MAX_HR_PLOT_POINTS] = {};
  int hrPlotMax[MAX_HR_PLOT_POINTS] = {};
  int hrPlotCount = 0;
  time_t hrPlotWindowStart = 0;
  time_t hrPlotWindowEnd = 0;
  int efficiency      = -1;
  int timeInBed       = -1;
  int awakeTime       = -1;
  int totalSleep      = -1;
  int lightSleep      = -1;
  int remSleep        = -1;
  int deepSleep       = -1;
  String bedtimeStart;
  String bedtimeEnd;
  String sleepDay;

  // 15-day sleep trends
  SleepTrend trends[MAX_TREND_DAYS];
  int trendCount = 0;

  // Averages (computed from trends)
  int avgSleepScore  = -1;
  int avgTimeInBed   = -1;
  int avgAwakeTime   = -1;
  int avgTotalSleep  = -1;
  int avgLightSleep  = -1;
  int avgRemSleep    = -1;
  int avgDeepSleep   = -1;

  // Meta
  String date;
  String lastUpdated;
};

bool loadTokens();
bool refreshAccessToken();
bool hasAccessToken();
bool useSeedRefreshToken();
String apiGet(const String& path);
DashboardData fetchDashboardData(const String& today, const String& tenDaysAgo);

#endif
