#include "oura_utils.h"

#include <stdint.h>
#include <stdio.h>

bool ouraIsUrlUnreserved(char c) {
  return (c >= 'A' && c <= 'Z') ||
         (c >= 'a' && c <= 'z') ||
         (c >= '0' && c <= '9') ||
         c == '-' || c == '_' || c == '.' || c == '~';
}

size_t ouraUrlEncodedLength(const char* value) {
  if (!value) return 0;

  size_t required = 0;
  for (const unsigned char* p = (const unsigned char*)value; *p; p++) {
    required += ouraIsUrlUnreserved((char)*p) ? 1 : 3;
  }
  return required;
}

size_t ouraUrlEncode(const char* value, char* out, size_t outSize) {
  static const char hex[] = "0123456789ABCDEF";

  if (!value) {
    if (out && outSize > 0) out[0] = '\0';
    return 0;
  }

  size_t required = 0;
  size_t written = 0;
  const size_t writable = (out && outSize > 0) ? outSize - 1 : 0;

  for (const unsigned char* p = (const unsigned char*)value; *p; p++) {
    char encoded[3];
    size_t encodedLen = 1;
    if (ouraIsUrlUnreserved((char)*p)) {
      encoded[0] = (char)*p;
    } else {
      encoded[0] = '%';
      encoded[1] = hex[*p >> 4];
      encoded[2] = hex[*p & 0x0F];
      encodedLen = 3;
    }

    for (size_t i = 0; i < encodedLen && written < writable; i++) {
      out[written++] = encoded[i];
    }
    required += encodedLen;
  }

  if (out && outSize > 0) out[written] = '\0';
  return required;
}

static bool parseDate(const char* yyyyMmDd, int* y, unsigned* m, unsigned* d) {
  int parsedY = 0;
  int parsedM = 0;
  int parsedD = 0;
  int consumed = 0;
  if (!yyyyMmDd ||
      sscanf(yyyyMmDd, "%d-%d-%d%n", &parsedY, &parsedM, &parsedD, &consumed) != 3 ||
      yyyyMmDd[consumed] != '\0' ||
      parsedM < 1 || parsedM > 12 ||
      parsedD < 1 || parsedD > 31) {
    return false;
  }

  *y = parsedY;
  *m = (unsigned)parsedM;
  *d = (unsigned)parsedD;
  return true;
}

static int64_t daysFromCivil(int y, unsigned m, unsigned d) {
  y -= m <= 2;
  const int era = (y >= 0 ? y : y - 399) / 400;
  const unsigned yoe = (unsigned)(y - era * 400);
  const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  return era * 146097 + (int64_t)doe - 719468;
}

static void civilFromDays(int64_t z, int* y, unsigned* m, unsigned* d) {
  z += 719468;
  const int era = (z >= 0 ? z : z - 146096) / 146097;
  const unsigned doe = (unsigned)(z - era * 146097);
  const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
  const int computedY = (int)yoe + era * 400;
  const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
  const unsigned mp = (5 * doy + 2) / 153;

  *d = doy - (153 * mp + 2) / 5 + 1;
  *m = mp + (mp < 10 ? 3 : -9);
  *y = computedY + (*m <= 2);
}

bool ouraShiftDate(const char* yyyyMmDd, int dayDelta, char* out, size_t outSize) {
  int y = 0;
  unsigned m = 0;
  unsigned d = 0;
  if (!out || outSize < 11 || !parseDate(yyyyMmDd, &y, &m, &d)) {
    return false;
  }

  int shiftedY = 0;
  unsigned shiftedM = 0;
  unsigned shiftedD = 0;
  civilFromDays(daysFromCivil(y, m, d) + dayDelta, &shiftedY, &shiftedM, &shiftedD);
  snprintf(out, outSize, "%04d-%02u-%02u", shiftedY, shiftedM, shiftedD);
  return true;
}

bool ouraIsWithinRefreshWindowHour(int hour, int startHour, int endHour) {
  return hour >= startHour && hour < endHour;
}

int ouraRefreshIntervalMinutesForHour(
  int hour,
  int startHour,
  int endHour,
  int activeIntervalMinutes,
  int inactiveIntervalMinutes
) {
  return ouraIsWithinRefreshWindowHour(hour, startHour, endHour)
    ? activeIntervalMinutes
    : inactiveIntervalMinutes;
}
