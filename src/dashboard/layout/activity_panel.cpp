#include "dashboard/layout/activity_panel.h"

#include "dashboard_color_config.h"
#include "dashboard_draw_utils.h"
#include "dashboard_icons.h"
#include "fonts/PoppinsRegular12pt7b.h"
#include "fonts/PoppinsRegular14pt7b.h"
#include "fonts/PoppinsRegular20pt7b.h"
#include "fonts/PoppinsRegular52pt7b.h"

void ActivityPanel::draw(const DashboardRenderContext& ctx) const {
  const int yNum = 72;
  const int yLbl = 98;
  const int c3 = 412;

  int activityScore = ctx.data.activityScore;
  uint16_t color = DashboardColorConfig::BLUE;
  if (activityScore < 70) {
    color = DashboardColorConfig::RED;
  } else if (activityScore >= 80) {
    color = DashboardColorConfig::GREEN;
  }
  drawIconActivity(c3 - 50, yNum - 20, 48, DashboardColorConfig::BLACK);
  drawText(c3 - 20, yNum, valOrDash(activityScore), &PoppinsRegular52pt7b, color);
  drawText(c3 - 28, yLbl, "Activity", &PoppinsRegular20pt7b, DashboardColorConfig::BLACK);
}
