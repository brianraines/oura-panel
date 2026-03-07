#include "dashboard/layout/resilience_panel.h"

#include "dashboard_color_config.h"
#include "dashboard_draw_utils.h"
#include "fonts/PoppinsRegular20pt7b.h"
#include "fonts/PoppinsRegular52pt7b.h"

void ResiliencePanel::draw(const DashboardRenderContext& ctx) const {
  const int yNum = 72;
  const int yLbl = 98;
  String level = ctx.data.resilienceLevel;
  uint16_t color = DashboardColorConfig::BLUE;

  String levelKey = level;
  levelKey.toLowerCase();
  if (levelKey == "exceptional" || levelKey == "strong") {
    color = DashboardColorConfig::GREEN;
  } else if (levelKey == "limited" || levelKey == "adequate") {
    color = DashboardColorConfig::RED;
  }

  if (level.length() == 0) level = "--";
  else if (level[0] >= 'a' && level[0] <= 'z') level[0] -= 32;
  drawTextR(DASHBOARD_W - 14, yNum, level, &PoppinsRegular52pt7b, color);
  drawTextR(DASHBOARD_W - 12, yLbl, "Resilience", &PoppinsRegular20pt7b, DashboardColorConfig::BLACK);
}
