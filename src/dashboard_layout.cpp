#include "dashboard_layout.h"

#include "dashboard/layout/dashboard_render_context.h"
#include "dashboard/layout/footer_section.h"
#include "dashboard/layout/header_section.h"
#include "dashboard/layout/last_night_section.h"
#include "dashboard/layout/panels_section.h"
#include "dashboard/layout/sleep_trend_section.h"
#include "dashboard/layout/vitals_section.h"

#include "dashboard_draw_utils.h"

void drawDashboardLayout(const DashboardData& data, int battPct) {
  Serial.println("[EPD] Drawing dashboard...");
  DashboardRenderContext ctx{data, battPct};
  HeaderSection header;
  PanelsSection panels;
  LastNightSection lastNight;
  VitalsSection vitals;
  SleepTrendSection sleepTrend;
  FooterSection footer;

  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    header.draw(ctx);
    panels.draw(ctx);
    lastNight.draw(ctx);
    vitals.draw(ctx);
    sleepTrend.draw(ctx);
    footer.draw(ctx);
  } while (display.nextPage());
  Serial.println("[EPD] Done");
}
