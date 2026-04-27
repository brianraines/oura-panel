#ifndef OURA_UTILS_H
#define OURA_UTILS_H

#include <stddef.h>

bool ouraIsUrlUnreserved(char c);
size_t ouraUrlEncodedLength(const char* value);
size_t ouraUrlEncode(const char* value, char* out, size_t outSize);

bool ouraShiftDate(const char* yyyyMmDd, int dayDelta, char* out, size_t outSize);

bool ouraIsWithinRefreshWindowHour(int hour, int startHour, int endHour);
int ouraRefreshIntervalMinutesForHour(
  int hour,
  int startHour,
  int endHour,
  int activeIntervalMinutes,
  int inactiveIntervalMinutes
);

#endif
