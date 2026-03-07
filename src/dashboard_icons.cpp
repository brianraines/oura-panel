#include "dashboard_icons.h"

#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_7c.h>

#include "icons/feather/black/12x12/activity12x12.h"
#include "icons/feather/black/12x12/clock12x12.h"
#include "icons/feather/black/12x12/heart12x12.h"
#include "icons/feather/black/12x12/moon12x12.h"
#include "icons/feather/black/12x12/sunrise12x12.h"
#include "icons/feather/black/12x12/sunset12x12.h"
#include "icons/feather/black/12x12/watch12x12.h"
#include "icons/feather/black/12x12/wind12x12.h"
#include "icons/feather/black/12x12/zap12x12.h"
#include "icons/feather/black/16x16/activity16x16.h"
#include "icons/feather/black/16x16/clock16x16.h"
#include "icons/feather/black/16x16/heart16x16.h"
#include "icons/feather/black/16x16/moon16x16.h"
#include "icons/feather/black/16x16/sunrise16x16.h"
#include "icons/feather/black/16x16/sunset16x16.h"
#include "icons/feather/black/16x16/watch16x16.h"
#include "icons/feather/black/16x16/wind16x16.h"
#include "icons/feather/black/16x16/zap16x16.h"
#include "icons/feather/black/24x24/activity24x24.h"
#include "icons/feather/black/24x24/clock24x24.h"
#include "icons/feather/black/24x24/heart24x24.h"
#include "icons/feather/black/24x24/moon24x24.h"
#include "icons/feather/black/24x24/sunrise24x24.h"
#include "icons/feather/black/24x24/sunset24x24.h"
#include "icons/feather/black/24x24/watch24x24.h"
#include "icons/feather/black/24x24/wind24x24.h"
#include "icons/feather/black/24x24/zap24x24.h"
#include "icons/feather/black/32x32/activity32x32.h"
#include "icons/feather/black/32x32/clock32x32.h"
#include "icons/feather/black/32x32/heart32x32.h"
#include "icons/feather/black/32x32/moon32x32.h"
#include "icons/feather/black/32x32/sunrise32x32.h"
#include "icons/feather/black/32x32/sunset32x32.h"
#include "icons/feather/black/32x32/watch32x32.h"
#include "icons/feather/black/32x32/wind32x32.h"
#include "icons/feather/black/32x32/zap32x32.h"
#include "icons/feather/black/48x48/activity48x48.h"
#include "icons/feather/black/48x48/clock48x48.h"
#include "icons/feather/black/48x48/heart48x48.h"
#include "icons/feather/black/48x48/moon48x48.h"
#include "icons/feather/black/48x48/sunrise48x48.h"
#include "icons/feather/black/48x48/sunset48x48.h"
#include "icons/feather/black/48x48/watch48x48.h"
#include "icons/feather/black/48x48/wind48x48.h"
#include "icons/feather/black/48x48/zap48x48.h"
#include "icons/feather/black/64x64/activity64x64.h"
#include "icons/feather/black/64x64/clock64x64.h"
#include "icons/feather/black/64x64/heart64x64.h"
#include "icons/feather/black/64x64/moon64x64.h"
#include "icons/feather/black/64x64/sunrise64x64.h"
#include "icons/feather/black/64x64/sunset64x64.h"
#include "icons/feather/black/64x64/watch64x64.h"
#include "icons/feather/black/64x64/wind64x64.h"
#include "icons/feather/black/64x64/zap64x64.h"
#include "icons/feather/black/96x96/activity96x96.h"
#include "icons/feather/black/96x96/clock96x96.h"
#include "icons/feather/black/96x96/heart96x96.h"
#include "icons/feather/black/96x96/moon96x96.h"
#include "icons/feather/black/96x96/sunrise96x96.h"
#include "icons/feather/black/96x96/sunset96x96.h"
#include "icons/feather/black/96x96/watch96x96.h"
#include "icons/feather/black/96x96/wind96x96.h"
#include "icons/feather/black/96x96/zap96x96.h"
#include "icons/feather/black/128x128/activity128x128.h"
#include "icons/feather/black/128x128/clock128x128.h"
#include "icons/feather/black/128x128/heart128x128.h"
#include "icons/feather/black/128x128/moon128x128.h"
#include "icons/feather/black/128x128/sunrise128x128.h"
#include "icons/feather/black/128x128/sunset128x128.h"
#include "icons/feather/black/128x128/watch128x128.h"
#include "icons/feather/black/128x128/wind128x128.h"
#include "icons/feather/black/128x128/zap128x128.h"
#include "icons/feather/black/160x160/activity160x160.h"
#include "icons/feather/black/160x160/clock160x160.h"
#include "icons/feather/black/160x160/heart160x160.h"
#include "icons/feather/black/160x160/moon160x160.h"
#include "icons/feather/black/160x160/sunrise160x160.h"
#include "icons/feather/black/160x160/sunset160x160.h"
#include "icons/feather/black/160x160/watch160x160.h"
#include "icons/feather/black/160x160/wind160x160.h"
#include "icons/feather/black/160x160/zap160x160.h"
#include "icons/feather/black/196x196/activity196x196.h"
#include "icons/feather/black/196x196/clock196x196.h"
#include "icons/feather/black/196x196/heart196x196.h"
#include "icons/feather/black/196x196/moon196x196.h"
#include "icons/feather/black/196x196/sunrise196x196.h"
#include "icons/feather/black/196x196/sunset196x196.h"
#include "icons/feather/black/196x196/watch196x196.h"
#include "icons/feather/black/196x196/wind196x196.h"
#include "icons/feather/black/196x196/zap196x196.h"

extern GxEPD2_7C<GxEPD2_730c_ACeP_730, GxEPD2_730c_ACeP_730::HEIGHT / 4> display;

static void drawIconCentered(const unsigned char* bitmap, int w, int h,
                             int x, int y, uint16_t color) {
  int x0 = x - (w / 2);
  int y0 = y - (h / 2);
  display.drawBitmap(x0, y0, bitmap, w, h, color);
}

static int chooseIconSize(int requestedSize) {
  if (requestedSize <= 0) return 12;
  static const int kSizes[] = {12, 16, 24, 32, 48, 64, 96, 128, 160, 196};
  int best = kSizes[0];
  int bestDelta = abs(requestedSize - best);
  for (size_t i = 1; i < sizeof(kSizes) / sizeof(kSizes[0]); i++) {
    int delta = abs(requestedSize - kSizes[i]);
    if (delta < bestDelta) {
      best = kSizes[i];
      bestDelta = delta;
    }
  }
  return best;
}

static void drawMoonBySize(int requestedSize, int x, int y, uint16_t color) {
  switch (chooseIconSize(requestedSize)) {
    case 12: drawIconCentered(moon12x12, 12, 12, x, y, color); break;
    case 16: drawIconCentered(moon16x16, 16, 16, x, y, color); break;
    case 24: drawIconCentered(moon24x24, 24, 24, x, y, color); break;
    case 32: drawIconCentered(moon32x32, 32, 32, x, y, color); break;
    case 48: drawIconCentered(moon48x48, 48, 48, x, y, color); break;
    case 64: drawIconCentered(moon64x64, 64, 64, x, y, color); break;
    case 96: drawIconCentered(moon96x96, 96, 96, x, y, color); break;
    case 128: drawIconCentered(moon128x128, 128, 128, x, y, color); break;
    case 160: drawIconCentered(moon160x160, 160, 160, x, y, color); break;
    case 196: drawIconCentered(moon196x196, 196, 196, x, y, color); break;
    default: drawIconCentered(moon24x24, 24, 24, x, y, color); break;
  }
}

static void drawZapBySize(int requestedSize, int x, int y, uint16_t color) {
  switch (chooseIconSize(requestedSize)) {
    case 12: drawIconCentered(zap12x12, 12, 12, x, y, color); break;
    case 16: drawIconCentered(zap16x16, 16, 16, x, y, color); break;
    case 24: drawIconCentered(zap24x24, 24, 24, x, y, color); break;
    case 32: drawIconCentered(zap32x32, 32, 32, x, y, color); break;
    case 48: drawIconCentered(zap48x48, 48, 48, x, y, color); break;
    case 64: drawIconCentered(zap64x64, 64, 64, x, y, color); break;
    case 96: drawIconCentered(zap96x96, 96, 96, x, y, color); break;
    case 128: drawIconCentered(zap128x128, 128, 128, x, y, color); break;
    case 160: drawIconCentered(zap160x160, 160, 160, x, y, color); break;
    case 196: drawIconCentered(zap196x196, 196, 196, x, y, color); break;
    default: drawIconCentered(zap24x24, 24, 24, x, y, color); break;
  }
}

static void drawActivityBySize(int requestedSize, int x, int y, uint16_t color) {
  switch (chooseIconSize(requestedSize)) {
    case 12: drawIconCentered(activity12x12, 12, 12, x, y, color); break;
    case 16: drawIconCentered(activity16x16, 16, 16, x, y, color); break;
    case 24: drawIconCentered(activity24x24, 24, 24, x, y, color); break;
    case 32: drawIconCentered(activity32x32, 32, 32, x, y, color); break;
    case 48: drawIconCentered(activity48x48, 48, 48, x, y, color); break;
    case 64: drawIconCentered(activity64x64, 64, 64, x, y, color); break;
    case 96: drawIconCentered(activity96x96, 96, 96, x, y, color); break;
    case 128: drawIconCentered(activity128x128, 128, 128, x, y, color); break;
    case 160: drawIconCentered(activity160x160, 160, 160, x, y, color); break;
    case 196: drawIconCentered(activity196x196, 196, 196, x, y, color); break;
    default: drawIconCentered(activity24x24, 24, 24, x, y, color); break;
  }
}

static void drawHeartBySize(int requestedSize, int x, int y, uint16_t color) {
  switch (chooseIconSize(requestedSize)) {
    case 12: drawIconCentered(heart12x12, 12, 12, x, y, color); break;
    case 16: drawIconCentered(heart16x16, 16, 16, x, y, color); break;
    case 24: drawIconCentered(heart24x24, 24, 24, x, y, color); break;
    case 32: drawIconCentered(heart32x32, 32, 32, x, y, color); break;
    case 48: drawIconCentered(heart48x48, 48, 48, x, y, color); break;
    case 64: drawIconCentered(heart64x64, 64, 64, x, y, color); break;
    case 96: drawIconCentered(heart96x96, 96, 96, x, y, color); break;
    case 128: drawIconCentered(heart128x128, 128, 128, x, y, color); break;
    case 160: drawIconCentered(heart160x160, 160, 160, x, y, color); break;
    case 196: drawIconCentered(heart196x196, 196, 196, x, y, color); break;
    default: drawIconCentered(heart24x24, 24, 24, x, y, color); break;
  }
}

static void drawSunriseBySize(int requestedSize, int x, int y, uint16_t color) {
  switch (chooseIconSize(requestedSize)) {
    case 12: drawIconCentered(sunrise12x12, 12, 12, x, y, color); break;
    case 16: drawIconCentered(sunrise16x16, 16, 16, x, y, color); break;
    case 24: drawIconCentered(sunrise24x24, 24, 24, x, y, color); break;
    case 32: drawIconCentered(sunrise32x32, 32, 32, x, y, color); break;
    case 48: drawIconCentered(sunrise48x48, 48, 48, x, y, color); break;
    case 64: drawIconCentered(sunrise64x64, 64, 64, x, y, color); break;
    case 96: drawIconCentered(sunrise96x96, 96, 96, x, y, color); break;
    case 128: drawIconCentered(sunrise128x128, 128, 128, x, y, color); break;
    case 160: drawIconCentered(sunrise160x160, 160, 160, x, y, color); break;
    case 196: drawIconCentered(sunrise196x196, 196, 196, x, y, color); break;
    default: drawIconCentered(sunrise24x24, 24, 24, x, y, color); break;
  }
}

static void drawSunsetBySize(int requestedSize, int x, int y, uint16_t color) {
  switch (chooseIconSize(requestedSize)) {
    case 12: drawIconCentered(sunset12x12, 12, 12, x, y, color); break;
    case 16: drawIconCentered(sunset16x16, 16, 16, x, y, color); break;
    case 24: drawIconCentered(sunset24x24, 24, 24, x, y, color); break;
    case 32: drawIconCentered(sunset32x32, 32, 32, x, y, color); break;
    case 48: drawIconCentered(sunset48x48, 48, 48, x, y, color); break;
    case 64: drawIconCentered(sunset64x64, 64, 64, x, y, color); break;
    case 96: drawIconCentered(sunset96x96, 96, 96, x, y, color); break;
    case 128: drawIconCentered(sunset128x128, 128, 128, x, y, color); break;
    case 160: drawIconCentered(sunset160x160, 160, 160, x, y, color); break;
    case 196: drawIconCentered(sunset196x196, 196, 196, x, y, color); break;
    default: drawIconCentered(sunset24x24, 24, 24, x, y, color); break;
  }
}

static void drawWindBySize(int requestedSize, int x, int y, uint16_t color) {
  switch (chooseIconSize(requestedSize)) {
    case 12: drawIconCentered(wind12x12, 12, 12, x, y, color); break;
    case 16: drawIconCentered(wind16x16, 16, 16, x, y, color); break;
    case 24: drawIconCentered(wind24x24, 24, 24, x, y, color); break;
    case 32: drawIconCentered(wind32x32, 32, 32, x, y, color); break;
    case 48: drawIconCentered(wind48x48, 48, 48, x, y, color); break;
    case 64: drawIconCentered(wind64x64, 64, 64, x, y, color); break;
    case 96: drawIconCentered(wind96x96, 96, 96, x, y, color); break;
    case 128: drawIconCentered(wind128x128, 128, 128, x, y, color); break;
    case 160: drawIconCentered(wind160x160, 160, 160, x, y, color); break;
    case 196: drawIconCentered(wind196x196, 196, 196, x, y, color); break;
    default: drawIconCentered(wind24x24, 24, 24, x, y, color); break;
  }
}

static void drawClockBySize(int requestedSize, int x, int y, uint16_t color) {
  switch (chooseIconSize(requestedSize)) {
    case 12: drawIconCentered(clock12x12, 12, 12, x, y, color); break;
    case 16: drawIconCentered(clock16x16, 16, 16, x, y, color); break;
    case 24: drawIconCentered(clock24x24, 24, 24, x, y, color); break;
    case 32: drawIconCentered(clock32x32, 32, 32, x, y, color); break;
    case 48: drawIconCentered(clock48x48, 48, 48, x, y, color); break;
    case 64: drawIconCentered(clock64x64, 64, 64, x, y, color); break;
    case 96: drawIconCentered(clock96x96, 96, 96, x, y, color); break;
    case 128: drawIconCentered(clock128x128, 128, 128, x, y, color); break;
    case 160: drawIconCentered(clock160x160, 160, 160, x, y, color); break;
    case 196: drawIconCentered(clock196x196, 196, 196, x, y, color); break;
    default: drawIconCentered(clock24x24, 24, 24, x, y, color); break;
  }
}

static void drawWatchBySize(int requestedSize, int x, int y, uint16_t color) {
  switch (chooseIconSize(requestedSize)) {
    case 12: drawIconCentered(watch12x12, 12, 12, x, y, color); break;
    case 16: drawIconCentered(watch16x16, 16, 16, x, y, color); break;
    case 24: drawIconCentered(watch24x24, 24, 24, x, y, color); break;
    case 32: drawIconCentered(watch32x32, 32, 32, x, y, color); break;
    case 48: drawIconCentered(watch48x48, 48, 48, x, y, color); break;
    case 64: drawIconCentered(watch64x64, 64, 64, x, y, color); break;
    case 96: drawIconCentered(watch96x96, 96, 96, x, y, color); break;
    case 128: drawIconCentered(watch128x128, 128, 128, x, y, color); break;
    case 160: drawIconCentered(watch160x160, 160, 160, x, y, color); break;
    case 196: drawIconCentered(watch196x196, 196, 196, x, y, color); break;
    default: drawIconCentered(watch24x24, 24, 24, x, y, color); break;
  }
}

void drawIconMoon(int x, int y, int r, uint16_t color) {
  drawMoonBySize(r, x, y, color);
}

void drawIconZap(int x, int y, int s, uint16_t color) {
  drawZapBySize(s, x, y, color);
}

void drawIconActivity(int x, int y, int s, uint16_t color) {
  drawActivityBySize(s, x, y, color);
}

void drawIconHeart(int x, int y, int s, uint16_t color) {
  drawHeartBySize(s, x, y, color);
}

void drawIconSunrise(int x, int y, int s, uint16_t color) {
  drawSunriseBySize(s, x, y, color);
}

void drawIconSunset(int x, int y, int s, uint16_t color) {
  drawSunsetBySize(s, x, y, color);
}

void drawIconWind(int x, int y, int s, uint16_t color) {
  drawWindBySize(s, x, y, color);
}

void drawIconClock(int x, int y, int s, uint16_t color) {
  drawClockBySize(s, x, y, color);
}

void drawIconWatch(int x, int y, int s, uint16_t color) {
  drawWatchBySize(s, x, y, color);
}
