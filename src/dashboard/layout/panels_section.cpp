#include "dashboard/layout/panels_section.h"

void PanelsSection::draw(const DashboardRenderContext& ctx) const {
  sleep.draw(ctx);
  readiness.draw(ctx);
  activity.draw(ctx);
  resilience.draw(ctx);
}
