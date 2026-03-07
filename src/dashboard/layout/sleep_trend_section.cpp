#include "dashboard/layout/sleep_trend_section.h"

#include "dashboard_color_config.h"
#include "dashboard_draw_utils.h"
#include "fonts/PoppinsRegular12pt7b.h"
#include "fonts/PoppinsRegular24pt7b.h"

void SleepTrendSection::draw(const DashboardRenderContext& ctx) const {
  const DashboardData& d = ctx.data;
  const int y0 = 292;
  drawText(15, y0 + 20, "Sleep Trend", &PoppinsRegular24pt7b, DashboardColorConfig::BLACK);
  drawText(160, y0 + 20, "(" + String(d.trendCount) + " days)", &PoppinsRegular12pt7b, DashboardColorConfig::BLACK);
  graph.draw(ctx);
  avg.draw(ctx);
}
