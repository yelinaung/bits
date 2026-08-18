#include "swap_int_exercise.h"
#include "vendor/munit/munit.h"
#include <stddef.h>
#include <stdio.h>

munit_case(RUN, test_swap_ints, {
  int a = 5;
  int b = 6;

  swap_ints(&a, &b);

  assert_int(a, ==, 6, "a is now 6");
  assert_int(b, ==, 5, "b is now 5");
});

munit_case(SUBMIT, test_swap_ints_same, {
  int a = 5;

  swap_ints(&a, &a);

  assert_int(a, ==, 5, "a is still 5");
});

int main() {
  MunitTest tests[] = {
      munit_test("/swap_ints", test_swap_ints),
      munit_test("/swap_ints_same", test_swap_ints_same),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("void-pointer", tests);
  return munit_suite_main(&suite, NULL, 0, NULL);
}
