#include "assert.h"
#include "snek_obj.h"
#include "vendor/munit/munit.h"
#include <stdlib.h>

munit_case(RUN, test_inc_refcount, {
  snek_object_t *obj = new_snek_integer(10);
  assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");

  refcount_inc(obj);
  assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

  free(obj);
});

munit_case(SUBMIT, test_inc_refcount_more, {
  snek_object_t *obj = new_snek_float(4.20);
  assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");

  refcount_inc(obj);
  refcount_inc(obj);
  refcount_inc(obj);
  refcount_inc(obj);
  refcount_inc(obj);
  assert_int(obj->refcount, ==, 6, "Refcount should be incremented to 6");

  free(obj);
});

munit_case(SUBMIT, test_null_obj, {
  refcount_inc(NULL);
  assert(1);
});

int main() {
  MunitTest tests[] = {
      munit_test("/test_inc_refcount", test_inc_refcount),
      munit_test("/test_inc_refcount_more", test_inc_refcount_more),
      munit_test("/test_null_obj", test_null_obj),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("refcount", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
