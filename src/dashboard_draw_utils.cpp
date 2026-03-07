#include "dashboard_draw_utils.h"

String fmtDur(int sec) {
  if (sec < 0) return "--";
  int h = sec / 3600;
  int m = (sec % 3600) / 60;
  if (h == 0) return String(m) + "m";
  if (m == 0) return String(h) + "h";
  char buf[12];
  snprintf(buf, sizeof(buf), "%dh%02dm", h, m);
  return String(buf);
}

String fmtTime(const String& iso) {
  int ti = iso.indexOf('T');
  if (ti < 0) return "--";
  int h = iso.substring(ti + 1, ti + 3).toInt();
  String m = iso.substring(ti + 4, ti + 6);
  bool pm = h >= 12;
  if (h > 12) h -= 12;
  if (h == 0) h = 12;
  char buf[10];
  snprintf(buf, sizeof(buf), "%d:%s%s", h, m.c_str(), pm ? "p" : "a");
  return String(buf);
}

String dayAbbrev(const String& yyyyMmDd) {
  int y = 0, m = 0, d = 0;
  if (sscanf(yyyyMmDd.c_str(), "%d-%d-%d", &y, &m, &d) != 3) return "---";

  struct tm t = {};
  t.tm_year = y - 1900;
  t.tm_mon  = m - 1;
  t.tm_mday = d;
  if (mktime(&t) < 0) return "---";

  char buf[4];
  strftime(buf, sizeof(buf), "%a", &t);
  return String(buf);
}

String fmtSteps(int val) {
  if (val < 0) return "--";
  if (val < 1000) return String(val);
  char buf[12];
  snprintf(buf, sizeof(buf), "%d,%03d", val / 1000, val % 1000);
  return String(buf);
}

String valOrDash(int v) {
  return (v >= 0) ? String(v) : String("--");
}

void drawText(int x, int y, const String& text,
              const GFXfont* font, uint16_t color) {
  display.setFont(font);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.print(text);
}

void drawTextR(int x, int y, const String& text,
               const GFXfont* font, uint16_t color) {
  display.setFont(font);
  int16_t x1, y1; uint16_t tw, th;
  display.getTextBounds(text, 0, 0, &x1, &y1, &tw, &th);
  display.setTextColor(color);
  display.setCursor(x - tw, y);
  display.print(text);
}

void drawTextC(int cx, int y, const String& text,
               const GFXfont* font, uint16_t color) {
  display.setFont(font);
  int16_t x1, y1; uint16_t tw, th;
  display.getTextBounds(text, 0, 0, &x1, &y1, &tw, &th);
  display.setTextColor(color);
  display.setCursor(cx - (int)tw / 2, y);
  display.print(text);
}
