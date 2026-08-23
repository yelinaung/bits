#include "snek_obj.h"
#include "vendor/munit/munit.h"

munit_case(RUN, test_positive, {
  snek_object_t *int_object = new_snek_integer(42);
  assert_int(int_object->data.v_int, ==, 42, "must allow positive numbers");

  free(int_object);
});

munit_case(RUN, test_zero, {
  snek_object_t *int_object = new_snek_integer(0);

  assert_int(int_object->kind, ==, INTEGER, "must be INTEGER type");
  assert_int(int_object->data.v_int, ==, 0, "must equal zero");

  free(int_object);
});

munit_case(SUBMIT, test_negative, {
  snek_object_t *int_object = new_snek_integer(-5);

  assert_int(int_object->kind, ==, INTEGER, "must be INTEGER type");
  assert_int(int_object->data.v_int, ==, -5, "must allow negative numbers");

  free(int_object);
});

int main() {
  MunitTest tests[] = {
      munit_test("/positive", test_positive),
      munit_test("/zero", test_zero),
      munit_test("/negative", test_negative),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("object-integer", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
