#include "snekstack.h"
#include "vendor/bootlib.h"
#include "vendor/munit/munit.h"
#include <assert.h>

munit_case(RUN, create_stack, {
  stack_t *s = stack_new(10);
  assert_int(s->capacity, ==, 10, "Sets capacity to 10");
  assert_int(s->count, ==, 0, "No elements in the stack yet");
  assert_ptr_not_null(s->data, "Allocates the stack data");

  // Clean up our allocated data.
  free(s->data);
  free(s);

  // Should be nothing left that is allocated.
  assert(boot_all_freed());
});

munit_case(RUN, push_stack, {
  stack_t *s = stack_new(2);
  assert_ptr_not_null(s, "Must allocate a new stack");

  assert_int(s->capacity, ==, 2, "Sets capacity to 2");
  assert_int(s->count, ==, 0, "No elements in the stack yet");
  assert_ptr_not_null(s->data, "Allocates the stack data");

  int a = 1;

  stack_push(s, &a);
  stack_push(s, &a);

  assert_int(s->capacity, ==, 2, "Sets capacity to 2");
  assert_int(s->count, ==, 2, "2 elements in the stack");
  assert_ptr_equal(s->data[0], &a, "element inserted into stack");

  // Clean up our allocated data.
  free(s->data);
  free(s);

  // Should be nothing left that is allocated.
  assert(boot_all_freed());
});

munit_case(RUN, push_double_capacity, {
  stack_t *s = stack_new(2);
  assert_ptr_not_null(s, "Must allocate a new stack");

  assert_int(s->capacity, ==, 2, "Sets capacity to 2");
  assert_int(s->count, ==, 0, "No elements in the stack yet");
  assert_ptr_not_null(s->data, "Allocates the stack data");

  int a = 1;

  stack_push(s, &a);
  stack_push(s, &a);

  assert_int(s->capacity, ==, 2, "Sets capacity to 2");
  assert_int(s->count, ==, 2, "2 elements in the stack");

  stack_push(s, &a);
  assert_int(s->capacity, ==, 4, "Capacity is doubled");
  assert_int(s->count, ==, 3, "3 elements in the stack");

  assert_size(boot_last_realloc_size(), ==, 4 * sizeof(void *),
              "realloc requested correct size");

  // Should reallocate memory.
  assert_int_equal(boot_realloc_count(), 1, "Must reallocate memory for stack");

  // Clean up our allocated data.
  free(s->data);
  free(s);

  // Should be nothing left that is allocated.
  assert(boot_all_freed());
});

munit_case(SUBMIT, push_multiple_values, {
  stack_t *s = stack_new(2);
  assert_ptr_not_null(s, "Must allocate a new stack");

  int one = 1;
  int two = 2;
  int three = 3;

  stack_push(s, &one);
  stack_push(s, &two);
  stack_push(s, &three);

  assert_int(s->capacity, ==, 4, "Capacity is doubled");
  assert_int(s->count, ==, 3, "3 elements in the stack");
  assert_ptr_equal(s->data[0], &one, "first element is preserved");
  assert_ptr_equal(s->data[1], &two, "second element is preserved");
  assert_ptr_equal(s->data[2], &three, "third element is inserted");

  // Clean up our allocated data.
  free(s->data);
  free(s);

  // Should be nothing left that is allocated.
  assert(boot_all_freed());
});

int main() {
  MunitTest tests[] = {
      munit_test("/create_stack", create_stack),
      munit_test("/push_stack", push_stack),
      munit_test("/push_double_capacity", push_double_capacity),
      munit_test("/push_multiple_values", push_multiple_values),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("snekstack", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
