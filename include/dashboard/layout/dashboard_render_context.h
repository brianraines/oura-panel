#ifndef DASHBOARD_RENDER_CONTEXT_H
#define DASHBOARD_RENDER_CONTEXT_H

#include "oura_api.h"

struct DashboardRenderContext {
  const DashboardData& data;
  int battPct;
};

static const int DASHBOARD_W = 800;
static const int DASHBOARD_H = 480;

#endif
