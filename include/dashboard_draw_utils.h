#ifndef DASHBOARD_DRAW_UTILS_H
#define DASHBOARD_DRAW_UTILS_H

#include <Arduino.h>
#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_7c.h>

extern GxEPD2_7C<GxEPD2_730c_ACeP_730, GxEPD2_730c_ACeP_730::HEIGHT / 4> display;

String fmtDur(int sec);
String fmtTime(const String& iso);
String dayAbbrev(const String& yyyyMmDd);
String fmtSteps(int val);
String valOrDash(int v);

void drawText(int x, int y, const String& text, const GFXfont* font, uint16_t color);
void drawTextR(int x, int y, const String& text, const GFXfont* font, uint16_t color);
void drawTextC(int cx, int y, const String& text, const GFXfont* font, uint16_t color);

#endif
