#include "snekstack.h"
#include "vendor/bootlib.h"
#include "vendor/munit/munit.h"
#include <assert.h>

munit_case(RUN, pop_stack, {
  stack_t *s = stack_new(2);
  assert_ptr_not_null(s, "Must allocate a new stack");

  assert_int(s->capacity, ==, 2, "Sets capacity to 2");
  assert_int(s->count, ==, 0, "No elements in the stack yet");
  assert_ptr_not_null(s->data, "Allocates the stack data");

  int one = 1;
  int two = 2;
  int three = 3;

  stack_push(s, &one);
  stack_push(s, &two);

  assert_int(s->capacity, ==, 2, "Sets capacity to 2");
  assert_int(s->count, ==, 2, "2 elements in the stack");

  stack_push(s, &three);
  assert_int(s->capacity, ==, 4, "Capacity is doubled");
  assert_int(s->count, ==, 3, "3 elements in the stack");

  int *popped = stack_pop(s);
  assert_int(*popped, ==, three, "Should pop the last element");

  popped = stack_pop(s);
  assert_int(*popped, ==, two, "Should pop the last element");

  popped = stack_pop(s);
  assert_int(*popped, ==, one, "Should pop the only remaining element");

  popped = stack_pop(s);
  assert_null(popped, "No remaining elements");

  // Clean up our allocated data.
  free(s->data);
  free(s);

  // Should be nothing left that is allocated.
  assert(boot_all_freed());
});

munit_case(SUBMIT, pop_stack_empty, {
  stack_t *s = stack_new(2);
  assert_ptr_not_null(s, "Must allocate a new stack");

  assert_int(s->capacity, ==, 2, "Sets capacity to 2");
  assert_int(s->count, ==, 0, "No elements in the stack yet");
  assert_ptr_not_null(s->data, "Allocates the stack data");

  int *popped = stack_pop(s);
  assert_null(popped, "Should return null when popping an empty stack");

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

  stack_push(s, &a);
  assert_int(s->capacity, ==, 4, "Capacity is doubled");
  assert_int(s->count, ==, 3, "3 elements in the stack");

  // Clean up our allocated data.
  free(s->data);
  free(s);

  // Should be nothing left that is allocated.
  assert(boot_all_freed());
});

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

int main() {
  MunitTest tests[] = {
      munit_test("/create_stack", create_stack),
      munit_test("/push_stack", push_stack),
      munit_test("/pop_stack", pop_stack),
      munit_test("/pop_stack_empty", pop_stack_empty),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("snekstack", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
