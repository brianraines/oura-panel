#include "dashboard/layout/vitals_section.h"

#include "dashboard_color_config.h"
#include "dashboard_draw_utils.h"
#include "dashboard_icons.h"
#include "fonts/PoppinsRegular12pt7b.h"
#include "fonts/PoppinsRegular14pt7b.h"
#include "fonts/PoppinsRegular22pt7b.h"
#include "fonts/PoppinsRegular24pt7b.h"

void VitalsSection::draw(const DashboardRenderContext& ctx) const {
  const DashboardData& d = ctx.data;
  const int y0 = 112;
  const int vx = 540;
  drawText(vx, y0 + 24, "Vitals", &PoppinsRegular24pt7b, DashboardColorConfig::BLACK);

  const int lx1 = vx;
  const int lx2 = vx + 132;
  const int labelX1 = lx1 + 28;
  const int labelX2 = lx2 + 28;

  const int iconSize = 32;

  const int row1y = y0 + 48;
  const int row2y = y0 + 96;
  const int row3y = y0 + 142;

  const int icon1Y = row1y + 8;
  const int icon2Y = row2y + 8;
  const int icon3Y = row3y + 8;

  const int value1y = row1y + 22;
  const int value2y = row2y + 22;
  const int value3y = row3y + 22;
  const GFXfont* labelFontSize = &PoppinsRegular14pt7b;
  const GFXfont* valueFontSize = &PoppinsRegular22pt7b;

  typedef void (*IconDrawer)(int, int, int, uint16_t);
  auto drawMetric = [&](IconDrawer iconDrawer, int lx, int iconY, int labelX, int labelY, int valueY,
                        uint16_t iconColor,
                        const char* label, const String& value) {
    iconDrawer(lx + 6, iconY, iconSize, iconColor);
    drawText(labelX, labelY, label, labelFontSize, DashboardColorConfig::BLACK);
    drawText(labelX, valueY, value, valueFontSize, DashboardColorConfig::BLACK);
  };

  drawMetric(drawIconHeart, lx1, icon1Y, labelX1, row1y, value1y, DashboardColorConfig::RED, "Average HR",
             valOrDash(d.avgHeartRate));
  drawMetric(drawIconHeart, lx2, icon1Y, labelX2, row1y, value1y, DashboardColorConfig::RED, "Current HR",
             valOrDash(d.currentHeartRate));

  drawMetric(drawIconHeart, lx1, icon2Y, labelX1, row2y, value2y, DashboardColorConfig::RED, "Low HR",
             valOrDash(d.lowestHeartRate));
  drawMetric(drawIconHeart, lx2, icon2Y, labelX2, row2y, value2y, DashboardColorConfig::BLACK, "Heart Age",
             valOrDash(d.vascularAge));

  drawMetric(drawIconActivity, lx1, icon3Y, labelX1, row3y, value3y, DashboardColorConfig::BLACK, "Steps",
             fmtSteps(d.steps));
  drawMetric(drawIconWind, lx2, icon3Y, labelX2, row3y, value3y, DashboardColorConfig::BLACK, "Stress",
             valOrDash(d.stress));
}
