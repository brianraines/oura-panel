#ifndef DASHBOARD_COLOR_CONFIG_H
#define DASHBOARD_COLOR_CONFIG_H

#include <Arduino.h>
#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_7c.h>

// Centralized mapping from intended display colors to panel palette constants.
// These names follow the identifiers requested for project usage.
struct DashboardColorConfig {
  static constexpr uint16_t BLACK = GxEPD_BLACK;
  static constexpr uint16_t WHITE = GxEPD_WHITE;

  // Observed panel mapping:
  // GxEPD_ORANGE renders as green on this display.
  static constexpr uint16_t GREEN = GxEPD_ORANGE;
  // GxEPD_YELLOW renders as blue on this display.
  static constexpr uint16_t BLUE = GxEPD_YELLOW;
  // GxEPD_BLUE renders as red on this display.
  static constexpr uint16_t RED = GxEPD_BLUE;
  // GxEPD_GREEN renders as yellow on this display.
  static constexpr uint16_t YELLOW = GxEPD_GREEN;

  // No distinct true orange mapping has been confirmed yet.
  static constexpr uint16_t ORANGE = GxEPD_ORANGE;
  static constexpr uint16_t DARKGREY = GxEPD_DARKGREY;
  static constexpr uint16_t LIGHTGREY = GxEPD_LIGHTGREY;
};

#endif
