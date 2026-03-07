#include "dashboard/layout/last_night_stages_section.h"

#include "dashboard_color_config.h"
#include "dashboard_draw_utils.h"
#include "fonts/PoppinsRegular24pt7b.h"
#include "fonts/PoppinsRegular26pt7b.h"

void LastNightStagesSection::draw(const DashboardRenderContext& ctx) const {
  const DashboardData& d = ctx.data;
  const int y0 = 118;
  const int sx = 298;
  int rowY = y0 + 52;
  const int sw = 24;
  const int rowStep = 34;
  const int labelX = sx + 36;
  const int valueX = sx + 130;
  int iconY = rowY - 22;

  const GFXfont* labelFontSize = &PoppinsRegular24pt7b;
  const GFXfont* valueFontSize = &PoppinsRegular24pt7b;

  auto drawStageRow = [&](const char* label, const String& value, uint16_t stageColor) {
    display.fillRect(sx, iconY, sw, sw, stageColor);
    iconY += rowStep;
    drawText(labelX, rowY, label, labelFontSize, DashboardColorConfig::DARKGREY);
    drawText(valueX, rowY, value, valueFontSize, DashboardColorConfig::BLACK);
    rowY += rowStep;
  };

  drawStageRow("Awake", fmtDur(d.awakeTime), DashboardColorConfig::BLUE);
  drawStageRow("Light", fmtDur(d.lightSleep), DashboardColorConfig::GREEN);
  drawStageRow("REM", fmtDur(d.remSleep), DashboardColorConfig::YELLOW);
  drawStageRow("Deep", fmtDur(d.deepSleep), DashboardColorConfig::RED);
}
