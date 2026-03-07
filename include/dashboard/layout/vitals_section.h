#ifndef VITALS_SECTION_H
#define VITALS_SECTION_H

#include "dashboard/layout/dashboard_render_context.h"

class VitalsSection {
 public:
  void draw(const DashboardRenderContext& ctx) const;
};

#endif
