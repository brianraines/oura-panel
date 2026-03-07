#include "dashboard/layout/sleep_trend_graph_section.h"

#include "dashboard_color_config.h"
#include "dashboard_draw_utils.h"
#include "fonts/PoppinsRegular9pt7b.h"
#include "fonts/PoppinsRegular12pt7b.h"
#include "fonts/PoppinsRegular20pt7b.h"

void SleepTrendGraphSection::draw(const DashboardRenderContext& ctx) const {
  const DashboardData& d = ctx.data;
  if (d.trendCount == 0) {
    drawTextC(DASHBOARD_W / 2, y0 + 90, "No trend data", &PoppinsRegular9pt7b, DashboardColorConfig::BLACK);
    return;
  }

  int maxSec = 0;
  for (int i = 0; i < d.trendCount; i++) {
    if (d.trends[i].timeInBed > maxSec) maxSec = d.trends[i].timeInBed;
  }
  if (maxSec <= 0) maxSec = 36000;
  maxSec = ((maxSec / 3600) + 1) * 3600;

  int maxH = maxSec / 3600;
  for (int h = 0; h <= maxH; h += 2) {
    int py = gy + gh - (int)((long)h * 3600L * gh / maxSec);
    display.drawLine(gx - 3, py, gx, py, DashboardColorConfig::BLACK);
    char lbl[4];
    snprintf(lbl, sizeof(lbl), "%dh", h);
    drawTextR(gx - 5, py + 5, String(lbl), &PoppinsRegular9pt7b, DashboardColorConfig::BLACK);
  }

  display.drawLine(gx, gy, gx, gy + gh, DashboardColorConfig::BLACK);
  display.drawLine(gx, gy + gh, gx + gw, gy + gh, DashboardColorConfig::BLACK);

  int barSlot = gw / d.trendCount;
  int barW = barSlot - 6;
  if (barW < 10) barW = 10;

  for (int i = 0; i < d.trendCount; i++) {
    const SleepTrend& t = d.trends[i];
    int bx = gx + i * barSlot + 3;
    if (t.timeInBed <= 0) continue;

    auto scaleH = [&](int s) -> int { return (int)((long)max(0, s) * gh / maxSec); };
    int deepH  = scaleH(t.deepSleep);
    int remH   = scaleH(t.remSleep);
    int lightH = scaleH(t.lightSleep);
    int awakeH = scaleH(t.awakeTime);

    int by = gy + gh;
    if (deepH  > 0) { display.fillRect(bx, by - deepH,  barW, deepH,  DashboardColorConfig::RED);    by -= deepH;  }
    if (remH   > 0) { display.fillRect(bx, by - remH,   barW, remH,   DashboardColorConfig::YELLOW); by -= remH;   }
    if (lightH > 0) { display.fillRect(bx, by - lightH, barW, lightH, DashboardColorConfig::GREEN);  by -= lightH; }
    if (awakeH > 0) { display.fillRect(bx, by - awakeH, barW, awakeH, DashboardColorConfig::BLUE);   by -= awakeH; }
  }

  for (int i = 0; i < d.trendCount; i++) {
    int cx = gx + i * barSlot + 3 + barW / 2;
    drawTextC(cx, gy + gh + 14, dayAbbrev(d.trends[i].day), &PoppinsRegular12pt7b, DashboardColorConfig::BLACK);
  }

  for (int i = 0; i < d.trendCount; i++) {
    if (d.trends[i].score < 0) continue;
    int cx = gx + i * barSlot + 3 + barW / 2;
    int cy = gy + gh - (int)((long)d.trends[i].score * gh / 100);
    display.fillCircle(cx, cy, 3, DashboardColorConfig::BLACK);
    drawTextC(cx, max(gy + 12, cy + 28), String(d.trends[i].score), &PoppinsRegular20pt7b, DashboardColorConfig::WHITE);
    if (i > 0 && d.trends[i - 1].score >= 0) {
      int px = gx + (i - 1) * barSlot + 3 + barW / 2;
      int py = gy + gh - (int)((long)d.trends[i - 1].score * gh / 100);
      display.drawLine(px, py, cx, cy, DashboardColorConfig::BLACK);
    }
  }

  display.drawLine(gx + gw, gy, gx + gw, gy + gh, DashboardColorConfig::BLACK);
  for (int s = 0; s <= 100; s += 50) {
    int py = gy + gh - (int)((long)s * gh / 100);
    display.drawLine(gx + gw, py, gx + gw + 3, py, DashboardColorConfig::BLACK);
    drawText(gx + gw + 5, py + 5, String(s), &PoppinsRegular9pt7b, DashboardColorConfig::BLACK);
  }
}
