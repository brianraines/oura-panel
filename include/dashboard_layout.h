#ifndef DASHBOARD_LAYOUT_H
#define DASHBOARD_LAYOUT_H

#include <Arduino.h>
#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_7c.h>

#include "oura_api.h"

void drawDashboardLayout(const DashboardData& data, int battPct);

#endif
