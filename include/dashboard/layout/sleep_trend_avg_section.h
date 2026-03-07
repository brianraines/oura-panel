#ifndef SLEEP_TREND_AVG_SECTION_H
#define SLEEP_TREND_AVG_SECTION_H

#include "dashboard/layout/dashboard_render_context.h"

class SleepTrendAvgSection {
 public:
  void draw(const DashboardRenderContext& ctx) const;

 private:
  static const int y0 = 282;
  static const int legX = 660;
};

#endif
