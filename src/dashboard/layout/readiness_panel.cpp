#include "dashboard/layout/readiness_panel.h"

#include "dashboard_color_config.h"
#include "dashboard_draw_utils.h"
#include "dashboard_icons.h"
#include "fonts/PoppinsRegular12pt7b.h"
#include "fonts/PoppinsRegular20pt7b.h"
#include "fonts/PoppinsRegular52pt7b.h"

void ReadinessPanel::draw(const DashboardRenderContext& ctx) const {
  const int yNum = 72;
  const int yLbl = 98;
  const int c2 = 265;

  int readinessScore = ctx.data.readinessScore;
  uint16_t color = DashboardColorConfig::BLUE;
  if (readinessScore < 70) {
    color = DashboardColorConfig::RED;
  } else if (readinessScore >= 80) {
    color = DashboardColorConfig::GREEN;
  }

  drawIconZap(c2 - 50, yNum - 20, 48, DashboardColorConfig::BLACK);
  drawText(c2 - 20, yNum, valOrDash(readinessScore), &PoppinsRegular52pt7b, color);
  drawText(c2 - 72, yLbl, "Readiness", &PoppinsRegular20pt7b, DashboardColorConfig::DARKGREY);
}
