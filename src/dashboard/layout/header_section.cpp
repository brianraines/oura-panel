#include "dashboard/layout/header_section.h"

#include "dashboard_color_config.h"
#include "dashboard_draw_utils.h"
#include "fonts/PoppinsRegular18pt7b.h"
#include "fonts/PoppinsRegular12pt7b.h"
#include "fonts/PoppinsRegular9pt7b.h"

void HeaderSection::draw(const DashboardRenderContext& ctx) const {
  drawText(16, 20, "Dream Monitoring Station", &PoppinsRegular18pt7b, DashboardColorConfig::BLACK);
  drawTextR(380, 20, "(Last Sync: " + ctx.data.lastUpdated + ")", &PoppinsRegular12pt7b, DashboardColorConfig::DARKGREY);

  const int bx = DASHBOARD_W - 58;
  const int by = 10;
  uint16_t batteryColor = DashboardColorConfig::BLACK;
  if (ctx.battPct >= 70) {
    batteryColor = DashboardColorConfig::GREEN;
  } else if (ctx.battPct >= 20) {
    batteryColor = DashboardColorConfig::BLUE;
  } else if (ctx.battPct >= 0) {
    batteryColor = DashboardColorConfig::RED;
  }

  display.drawRect(bx, by, 18, 10, batteryColor);
  display.fillRect(bx + 18, by + 3, 2, 4, batteryColor);
  if (ctx.battPct >= 0) {
    int fillW = (int)((long)constrain(ctx.battPct, 0, 100) * 16 / 100);
    if (fillW > 0) display.fillRect(bx + 1, by + 1, fillW, 8, batteryColor);
    drawTextR(DASHBOARD_W - 10, 20, String(ctx.battPct) + "%", &PoppinsRegular12pt7b, DashboardColorConfig::BLACK);
  } else {
    drawTextR(DASHBOARD_W - 10, 20, "--", &PoppinsRegular12pt7b, DashboardColorConfig::BLACK);
  }
}
