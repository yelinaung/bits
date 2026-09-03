#include "fix_void_pointer.h"
#include "vendor/munit/munit.h"

munit_case(RUN, test_read_values_run, {
  int score = 7;
  double price = 12.5;

  munit_assert_int(read_int(&score), ==, 7,
                   "Input: int score = 7\nExpected: 7");
  munit_assert_double(read_double(&price), ==, 12.5,
                      "Input: double price = 12.5\nExpected: 12.5");
});

munit_case(RUN, test_update_values_run, {
  int score = 7;
  double price = 12.5;
  char grade = 'A';

  add_to_int(&score, 3);
  add_to_double(&price, 2.5);
  shift_char(&grade, 2);

  munit_assert_int(score, ==, 10,
                   "Input: int score = 7, amount = 3\nExpected: 10");
  munit_assert_double(
      price, ==, 15.0,
      "Input: double price = 12.5, amount = 2.5\nExpected: 15.0");
  munit_assert_char(grade, ==, 'C',
                    "Input: char grade = 'A', amount = 2\nExpected: 'C'");
});

munit_case(SUBMIT, test_edge_cases_submit, {
  int count = 0;
  double balance = 3.0;
  char letter = 'x';

  add_to_int(&count, -4);
  add_to_double(&balance, -1.5);
  shift_char(&letter, 1);

  munit_assert_int(count, ==, -4,
                   "Input: int count = 0, amount = -4\nExpected: -4");
  munit_assert_double(
      balance, ==, 1.5,
      "Input: double balance = 3.0, amount = -1.5\nExpected: 1.5");
  munit_assert_char(letter, ==, 'y',
                    "Input: char letter = 'x', amount = 1\nExpected: 'y'");
});

munit_case(SUBMIT, test_read_after_updates_submit, {
  int level = 20;
  double weight = 8.25;

  add_to_int(&level, 5);
  add_to_double(&weight, 0.75);

  munit_assert_int(read_int(&level), ==, 25,
                   "Input: int level = 20, then add 5\nExpected read: 25");
  munit_assert_double(
      read_double(&weight), ==, 9.0,
      "Input: double weight = 8.25, then add 0.75\nExpected read: 9.0");
});

munit_case(SUBMIT, test_happy_path_submit, {
  int hp = 100;
  double speed = 4.5;
  char rank = 'B';

  add_to_int(&hp, 25);
  add_to_double(&speed, 1.5);
  shift_char(&rank, 3);

  munit_assert_int(read_int(&hp), ==, 125,
                   "Input: int hp = 100, amount = 25\nExpected: 125");
  munit_assert_double(read_double(&speed), ==, 6.0,
                      "Input: double speed = 4.5, amount = 1.5\nExpected: 6.0");
  munit_assert_char(rank, ==, 'E',
                    "Input: char rank = 'B', amount = 3\nExpected: 'E'");
});

int main() {
  MunitTest tests[] = {
      munit_test("/run/read_values", test_read_values_run),
      munit_test("/run/update_values", test_update_values_run),
      munit_test("/submit/edge_cases", test_edge_cases_submit),
      munit_test("/submit/read_after_updates", test_read_after_updates_submit),
      munit_test("/submit/happy_path", test_happy_path_submit),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("void_pointer_helpers", tests);
  return munit_suite_main(&suite, NULL, 0, NULL);
}
