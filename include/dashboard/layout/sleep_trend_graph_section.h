#ifndef SLEEP_TREND_GRAPH_SECTION_H
#define SLEEP_TREND_GRAPH_SECTION_H

#include "dashboard/layout/dashboard_render_context.h"

class SleepTrendGraphSection {
 public:
  void draw(const DashboardRenderContext& ctx) const;

 private:
  static const int y0 = 302;
  static const int gx = 55;
  static const int gy = y0 + 32;
  static const int gw = 570;
  static const int gh = 125;
};

#endif
