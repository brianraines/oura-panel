#ifndef SLEEP_TREND_SECTION_H
#define SLEEP_TREND_SECTION_H

#include "dashboard/layout/sleep_trend_avg_section.h"
#include "dashboard/layout/sleep_trend_graph_section.h"

class SleepTrendSection {
 public:
  void draw(const DashboardRenderContext& ctx) const;

 private:
  SleepTrendGraphSection graph;
  SleepTrendAvgSection avg;
};

#endif
