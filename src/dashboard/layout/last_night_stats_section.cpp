#include "dashboard/layout/last_night_stats_section.h"

#include "dashboard_color_config.h"
#include "dashboard_draw_utils.h"
#include "dashboard_icons.h"
#include "fonts/PoppinsRegular12pt7b.h"
#include "fonts/PoppinsRegular14pt7b.h"
#include "fonts/PoppinsRegular16pt7b.h"
#include "fonts/PoppinsRegular18pt7b.h"
#include "fonts/PoppinsRegular22pt7b.h"
#include "fonts/PoppinsRegular24pt7b.h"
#include "fonts/PoppinsRegular9pt7b.h"

void LastNightStatsSection::draw(const DashboardRenderContext& ctx) const {
  const DashboardData& d = ctx.data;
  const int y0 = 112;
  String effStr = (d.efficiency >= 0) ? (String(d.efficiency) + "%") : "--";

  uint16_t efficiencyColor = DashboardColorConfig::BLUE;
  if (d.efficiency >= 80) {
    efficiencyColor = DashboardColorConfig::GREEN;
  } else {
    efficiencyColor = DashboardColorConfig::RED;
  }

  drawText(16, y0 + 24, "Last Night Efficiency", &PoppinsRegular24pt7b, DashboardColorConfig::DARKGREY);
  drawText(256, y0 + 24, effStr, &PoppinsRegular24pt7b, efficiencyColor);

  int lx1 = 30, lx2 = 175;
  int labelX1 = lx1 + 28;
  int labelX2 = lx2 + 28;

  int iconSize = 32;

  int row1y = y0 + 48, row2y = y0 + 96, row3y = y0 + 142;
  int icon1Y = row1y + 6;
  int icon2Y = row2y + 8;
  int icon3Y = row3y + 8;

  int value1y = row1y + 22;
  int value2y = row2y + 22;
  int value3y = row3y + 22;

  const GFXfont* labelFontSize = &PoppinsRegular14pt7b;
  const GFXfont* valueFontSize = &PoppinsRegular22pt7b;

  drawIconSunset(lx1 + 6, icon1Y, iconSize, DashboardColorConfig::BLACK);
  drawText(labelX1, row1y, "Bedtime", labelFontSize, DashboardColorConfig::BLACK);
  drawText(labelX1, value1y, fmtTime(d.bedtimeStart), valueFontSize, DashboardColorConfig::BLACK);
  drawIconSunrise(lx2 + 6, icon1Y, iconSize, DashboardColorConfig::BLACK);
  drawText(labelX2, row1y, "Wake", labelFontSize, DashboardColorConfig::BLACK);
  drawText(labelX2, value1y, fmtTime(d.bedtimeEnd), valueFontSize, DashboardColorConfig::BLACK);

  drawIconClock(lx1 + 6, icon2Y, iconSize, DashboardColorConfig::BLACK);
  drawText(labelX1, row2y, "Total Sleep", labelFontSize, DashboardColorConfig::BLACK);
  drawText(labelX1, value2y, fmtDur(d.totalSleep), valueFontSize, DashboardColorConfig::BLACK);
  drawIconClock(lx2 + 6, icon2Y, iconSize, DashboardColorConfig::BLACK);
  drawText(labelX2, row2y, "In Bed", labelFontSize, DashboardColorConfig::BLACK);
  drawText(labelX2, value2y, fmtDur(d.timeInBed), valueFontSize, DashboardColorConfig::BLACK);

  char breathBuf[8];
  if (d.avgBreath >= 0) snprintf(breathBuf, sizeof(breathBuf), "%.1f", d.avgBreath);
  else snprintf(breathBuf, sizeof(breathBuf), "--");
  drawIconWind(lx1 + 6, icon3Y, iconSize, DashboardColorConfig::BLUE);
  drawText(labelX1, row3y, "Breath", labelFontSize, DashboardColorConfig::BLACK);
  drawText(labelX1, value3y, String(breathBuf), valueFontSize, DashboardColorConfig::BLACK);
  drawIconHeart(lx2 + 6, icon3Y, iconSize, DashboardColorConfig::RED);
  drawText(labelX2, row3y, "RHR", labelFontSize, DashboardColorConfig::BLACK);
  drawText(labelX2, value3y, valOrDash(d.restingHeartRate), valueFontSize, DashboardColorConfig::BLACK);

}
