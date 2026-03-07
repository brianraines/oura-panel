#include "dashboard/layout/sleep_panel.h"

#include "dashboard_color_config.h"
#include "dashboard_draw_utils.h"
#include "dashboard_icons.h"
#include "fonts/PoppinsRegular20pt7b.h"
#include "fonts/PoppinsRegular52pt7b.h"

void SleepPanel::draw(const DashboardRenderContext& ctx) const {
  const int yNum = 72;
  const int yLbl = 98;
  const int c1 = 88;

  int sleepScore = ctx.data.sleepScore;
  uint16_t color = DashboardColorConfig::BLUE;
  if (sleepScore < 70) {
    color = DashboardColorConfig::RED;
  } else if (sleepScore >= 80) {
    color = DashboardColorConfig::GREEN;
  }

  drawIconMoon(c1 - 50, yNum - 20, 48, DashboardColorConfig::BLACK);
  drawText(c1 - 20, yNum, valOrDash(sleepScore), &PoppinsRegular52pt7b, color);
  drawText(c1 - 14, yLbl, "Sleep", &PoppinsRegular20pt7b, DashboardColorConfig::DARKGREY);
}
