#ifndef PANELS_SECTION_H
#define PANELS_SECTION_H

#include "dashboard/layout/activity_panel.h"
#include "dashboard/layout/readiness_panel.h"
#include "dashboard/layout/resilience_panel.h"
#include "dashboard/layout/sleep_panel.h"

class PanelsSection {
 public:
  void draw(const DashboardRenderContext& ctx) const;

 private:
  SleepPanel sleep;
  ReadinessPanel readiness;
  ActivityPanel activity;
  ResiliencePanel resilience;
};

#endif
