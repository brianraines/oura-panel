#include "dashboard/layout/sleep_trend_avg_section.h"

#include "dashboard_color_config.h"
#include "dashboard_draw_utils.h"
#include "fonts/PoppinsRegular12pt7b.h"
#include "fonts/PoppinsRegular14pt7b.h"
#include "fonts/PoppinsRegular9pt7b.h"
#include "fonts/PoppinsRegular20pt7b.h"
#include "fonts/PoppinsRegular24pt7b.h"

void SleepTrendAvgSection::draw(const DashboardRenderContext& ctx) const {
  const DashboardData& d = ctx.data;
  int legY = y0 + 62;
  const int legStep = 22;

  int labelX = legX + 24;
  int valueX = legX + 84;

  drawText(legX, legY - 18, "Averages", &PoppinsRegular24pt7b, DashboardColorConfig::BLACK);
  legY += 6;

  display.drawLine(legX, legY - 5, legX + 14, legY - 5, DashboardColorConfig::BLACK);
  drawText(labelX, legY, "Score", &PoppinsRegular14pt7b, DashboardColorConfig::BLACK);
  drawText(valueX, legY, valOrDash(d.avgSleepScore), &PoppinsRegular14pt7b, DashboardColorConfig::BLACK);
  legY += legStep;
  drawText(labelX, legY, "Sleep", &PoppinsRegular14pt7b, DashboardColorConfig::BLACK);
  drawText(valueX, legY, fmtDur(d.avgTotalSleep), &PoppinsRegular14pt7b, DashboardColorConfig::BLACK);
  legY += legStep;

  const uint16_t legColors[] = {
    DashboardColorConfig::BLUE,
    DashboardColorConfig::GREEN,
    DashboardColorConfig::YELLOW,
    DashboardColorConfig::RED
  };
  String legLabels[] = {
    "Awake",
    "Light",
    "REM",
    "Deep"
  };
  String legValues[] = {
    fmtDur(d.avgAwakeTime),
    fmtDur(d.avgLightSleep),
    fmtDur(d.avgRemSleep),
    fmtDur(d.avgDeepSleep)
  };
  for (int i = 0; i < 4; i++) {
    display.fillRect(legX, legY - 14, 16, 16, legColors[i]);
    drawText(labelX, legY, legLabels[i], &PoppinsRegular14pt7b, DashboardColorConfig::BLACK);
    drawText(valueX, legY, legValues[i], &PoppinsRegular14pt7b, DashboardColorConfig::BLACK);
    legY += legStep;
  }
}
