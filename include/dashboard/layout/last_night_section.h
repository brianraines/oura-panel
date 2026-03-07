#ifndef LAST_NIGHT_SECTION_H
#define LAST_NIGHT_SECTION_H

#include "dashboard/layout/last_night_stages_section.h"
#include "dashboard/layout/last_night_stats_section.h"

class LastNightSection {
 public:
  void draw(const DashboardRenderContext& ctx) const;

 private:
  LastNightStatsSection stats;
  LastNightStagesSection stages;
};

#endif
