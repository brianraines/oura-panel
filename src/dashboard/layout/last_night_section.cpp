#include "dashboard/layout/last_night_section.h"

void LastNightSection::draw(const DashboardRenderContext& ctx) const {
  stats.draw(ctx);
  stages.draw(ctx);
}
