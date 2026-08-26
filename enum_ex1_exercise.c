#include "enum_ex1.h"
#include "vendor/munit/munit.h"

munit_case(RUN, test_count_statuses_run_basic, {
  status_t items[] = {STATUS_PENDING, STATUS_DONE, STATUS_FAILED, STATUS_DONE};
  status_counts_t counts;

  count_statuses(items, 4, &counts);

  munit_assert_int(counts.pending, ==, 1,
                   "Input: [PENDING, DONE, FAILED, DONE]\nExpected pending: 1");
  munit_assert_int(counts.done, ==, 2,
                   "Input: [PENDING, DONE, FAILED, DONE]\nExpected done: 2");
  munit_assert_int(counts.failed, ==, 1,
                   "Input: [PENDING, DONE, FAILED, DONE]\nExpected failed: 1");
  munit_assert_int(counts.unknown, ==, 0,
                   "Input: [PENDING, DONE, FAILED, DONE]\nExpected unknown: 0");
});

munit_case(RUN, test_add_status_run_invalid_value, {
  status_counts_t counts;
  reset_counts(&counts);

  add_status(&counts, (status_t)99);
  add_status(&counts, STATUS_FAILED);

  munit_assert_int(counts.pending, ==, 0,
                   "Input: [99, FAILED]\nExpected pending: 0");
  munit_assert_int(counts.done, ==, 0, "Input: [99, FAILED]\nExpected done: 0");
  munit_assert_int(counts.failed, ==, 1,
                   "Input: [99, FAILED]\nExpected failed: 1");
  munit_assert_int(counts.unknown, ==, 1,
                   "Input: [99, FAILED]\nExpected unknown: 1");
});

munit_case(SUBMIT, test_count_statuses_submit_null_array, {
  status_counts_t counts;
  counts.pending = 7;
  counts.done = 7;
  counts.failed = 7;
  counts.unknown = 7;

  count_statuses(0, 3, &counts);

  munit_assert_int(counts.pending, ==, 0,
                   "Input: NULL array, length 3\nExpected pending: 0");
  munit_assert_int(counts.done, ==, 0,
                   "Input: NULL array, length 3\nExpected done: 0");
  munit_assert_int(counts.failed, ==, 0,
                   "Input: NULL array, length 3\nExpected failed: 0");
  munit_assert_int(counts.unknown, ==, 0,
                   "Input: NULL array, length 3\nExpected unknown: 0");
});

munit_case(SUBMIT, test_count_statuses_submit_zero_length, {
  status_t items[] = {STATUS_DONE, STATUS_DONE};
  status_counts_t counts;
  counts.pending = 4;
  counts.done = 4;
  counts.failed = 4;
  counts.unknown = 4;

  count_statuses(items, 0, &counts);

  munit_assert_int(counts.pending, ==, 0,
                   "Input: [DONE, DONE], length 0\nExpected pending: 0");
  munit_assert_int(counts.done, ==, 0,
                   "Input: [DONE, DONE], length 0\nExpected done: 0");
  munit_assert_int(counts.failed, ==, 0,
                   "Input: [DONE, DONE], length 0\nExpected failed: 0");
  munit_assert_int(counts.unknown, ==, 0,
                   "Input: [DONE, DONE], length 0\nExpected unknown: 0");
});

munit_case(SUBMIT, test_count_statuses_submit_mixed, {
  status_t items[] = {STATUS_DONE,   (status_t)42, STATUS_PENDING, STATUS_DONE,
                      STATUS_FAILED, (status_t)-1, STATUS_PENDING};
  status_counts_t counts;

  count_statuses(items, 7, &counts);

  munit_assert_int(counts.pending, ==, 2,
                   "Input: [DONE, 42, PENDING, DONE, FAILED, -1, "
                   "PENDING]\nExpected pending: 2");
  munit_assert_int(counts.done, ==, 2,
                   "Input: [DONE, 42, PENDING, DONE, FAILED, -1, "
                   "PENDING]\nExpected done: 2");
  munit_assert_int(counts.failed, ==, 1,
                   "Input: [DONE, 42, PENDING, DONE, FAILED, -1, "
                   "PENDING]\nExpected failed: 1");
  munit_assert_int(counts.unknown, ==, 2,
                   "Input: [DONE, 42, PENDING, DONE, FAILED, -1, "
                   "PENDING]\nExpected unknown: 2");
});

int main() {
  MunitTest tests[] = {
      munit_test("/run/count_statuses_basic", test_count_statuses_run_basic),
      munit_test("/run/add_status_invalid_value",
                 test_add_status_run_invalid_value),
      munit_test("/submit/count_statuses_null_array",
                 test_count_statuses_submit_null_array),
      munit_test("/submit/count_statuses_zero_length",
                 test_count_statuses_submit_zero_length),
      munit_test("/submit/count_statuses_mixed",
                 test_count_statuses_submit_mixed),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("count_statuses", tests);
  return munit_suite_main(&suite, NULL, 0, NULL);
}
