#include <unity.h>

#include <string.h>

#include "oura_utils.h"

// PlatformIO's native test runner does not build src/ when test_build_src=no.
#include "../../src/oura_utils.cpp"

void test_url_encode_leaves_unreserved_characters() {
  char out[64];
  size_t len = ouraUrlEncode("abcXYZ-_.~123", out, sizeof(out));
  TEST_ASSERT_EQUAL_STRING("abcXYZ-_.~123", out);
  TEST_ASSERT_EQUAL_UINT(strlen("abcXYZ-_.~123"), len);
}

void test_url_encode_escapes_form_reserved_characters() {
  char out[64];
  size_t len = ouraUrlEncode("a+b c&d=e%f", out, sizeof(out));
  TEST_ASSERT_EQUAL_STRING("a%2Bb%20c%26d%3De%25f", out);
  TEST_ASSERT_EQUAL_UINT(strlen("a%2Bb%20c%26d%3De%25f"), len);
}

void test_url_encode_reports_required_length_when_truncated() {
  char out[8];
  size_t len = ouraUrlEncode("a b+c", out, sizeof(out));
  TEST_ASSERT_EQUAL_STRING("a%20b%2", out);
  TEST_ASSERT_EQUAL_UINT(strlen("a%20b%2Bc"), len);
}

void test_shift_date_handles_month_and_leap_boundaries() {
  char out[11];
  TEST_ASSERT_TRUE(ouraShiftDate("2024-02-28", 1, out, sizeof(out)));
  TEST_ASSERT_EQUAL_STRING("2024-02-29", out);

  TEST_ASSERT_TRUE(ouraShiftDate("2024-03-01", -1, out, sizeof(out)));
  TEST_ASSERT_EQUAL_STRING("2024-02-29", out);

  TEST_ASSERT_TRUE(ouraShiftDate("2026-12-31", 1, out, sizeof(out)));
  TEST_ASSERT_EQUAL_STRING("2027-01-01", out);
}

void test_shift_date_rejects_invalid_input_or_small_output() {
  char out[11];
  TEST_ASSERT_FALSE(ouraShiftDate("not-a-date", 1, out, sizeof(out)));
  TEST_ASSERT_FALSE(ouraShiftDate("2026-01-01", 1, out, 10));
}

void test_refresh_window_boundaries() {
  TEST_ASSERT_FALSE(ouraIsWithinRefreshWindowHour(7, 8, 20));
  TEST_ASSERT_TRUE(ouraIsWithinRefreshWindowHour(8, 8, 20));
  TEST_ASSERT_TRUE(ouraIsWithinRefreshWindowHour(19, 8, 20));
  TEST_ASSERT_FALSE(ouraIsWithinRefreshWindowHour(20, 8, 20));
}

void test_refresh_interval_selection() {
  TEST_ASSERT_EQUAL_INT(30, ouraRefreshIntervalMinutesForHour(12, 8, 20, 30, 60));
  TEST_ASSERT_EQUAL_INT(60, ouraRefreshIntervalMinutesForHour(22, 8, 20, 30, 60));
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  UNITY_BEGIN();
  RUN_TEST(test_url_encode_leaves_unreserved_characters);
  RUN_TEST(test_url_encode_escapes_form_reserved_characters);
  RUN_TEST(test_url_encode_reports_required_length_when_truncated);
  RUN_TEST(test_shift_date_handles_month_and_leap_boundaries);
  RUN_TEST(test_shift_date_rejects_invalid_input_or_small_output);
  RUN_TEST(test_refresh_window_boundaries);
  RUN_TEST(test_refresh_interval_selection);
  return UNITY_END();
}
