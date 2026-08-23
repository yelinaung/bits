#include "assert.h"
#include "snek_obj.h"
#include "vendor/bootlib.h"
#include "vendor/munit/munit.h"
#include <stdlib.h>

munit_case(RUN, test_set, {
  snek_object_t *obj = new_snek_array(2);
  snek_object_t *first = new_snek_string("First");
  snek_object_t *second = new_snek_integer(3);

  assert(snek_array_set(obj, 0, first));
  assert(snek_array_set(obj, 1, second));

  assert_ptr(obj->data.v_array.elements[0], ==, first,
             "Should set the first element");
  assert_ptr(obj->data.v_array.elements[1], ==, second,
             "Should set the second element");

  free(first->data.v_string);
  free(first);
  free(second);
  free(obj->data.v_array.elements);
  free(obj);
  assert(boot_all_freed());
});

munit_case(RUN, test_set_outside_bounds, {
  snek_object_t *obj = new_snek_array(2);
  snek_object_t *outside = new_snek_string("First");

  assert(snek_array_set(obj, 1, outside));
  assert_false(snek_array_set(obj, 2, outside));
  assert_false(snek_array_set(obj, 100, outside));
  assert_ptr(obj->data.v_array.elements[1], ==, outside,
             "Should preserve existing elements");

  free(outside->data.v_string);
  free(outside);
  free(obj->data.v_array.elements);
  free(obj);
  assert(boot_all_freed());
});

munit_case(SUBMIT, test_set_rejects_invalid_inputs, {
  snek_object_t *array = new_snek_array(1);
  snek_object_t *value = new_snek_integer(3);
  snek_object_t *not_array = new_snek_integer(5);

  assert_false(snek_array_set(NULL, 0, value));
  assert_false(snek_array_set(array, 0, NULL));
  assert_false(snek_array_set(not_array, 0, value));

  free(value);
  free(not_array);
  free(array->data.v_array.elements);
  free(array);
  assert(boot_all_freed());
});

int main() {
  MunitTest tests[] = {
      munit_test("/set", test_set),
      munit_test("/set_outside", test_set_outside_bounds),
      munit_test("/set_invalid", test_set_rejects_invalid_inputs),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("object-array", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
