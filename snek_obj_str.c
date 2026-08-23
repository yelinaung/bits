#include "assert.h"
#include "snek_obj.h"
#include "vendor/bootlib.h"
#include "vendor/munit/munit.h"
#include <stdlib.h>

munit_case(RUN, test_str_copied, {
  char *input = "Hello, World!";
  snek_object_t *obj = new_snek_string(input);

  assert_int(obj->kind, ==, STRING, "Must be a string!");

  // Should not have pointers be the same, otherwise we didn't copy the value.
  assert_ptr_not_equal(obj->data.v_string, input,
                       "You need to copy the string.");

  // But should have the same data!
  //  This way the object can free it's own memory later.
  assert_string_equal(obj->data.v_string, input,
                      "Should copy string correctly");

  // Should allocate memory for the string with null terminator.
  assert_int_equal(boot_alloc_size(), 30, "Must allocate memory for string");

  // Free the string, and then free the object.
  free(obj->data.v_string);
  free(obj);
  assert(boot_all_freed());
});

int main() {
  MunitTest tests[] = {
      munit_test("/copies_value", test_str_copied),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("object-string", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
