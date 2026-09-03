#include "stack_ex.h"
#include "vendor/munit/munit.h"
#include <stdlib.h>

munit_case(RUN, test_stack_basic_run, {
  IntStack stack;
  stack_init(&stack, 2);

  int ok;
  int value;

  ok = stack_push(&stack, 10);
  munit_assert_int(ok, ==, 1,
                   "Input: push 10 on empty stack (capacity 2)\nExpected: push "
                   "returns 1 (success)\nActual:   push returned 0 (failure)");

  ok = stack_push(&stack, 20);
  munit_assert_int(ok, ==, 1,
                   "Input: push 20 after 10 on stack [10]\nExpected: push "
                   "returns 1 (success)\nActual:   push returned 0 (failure)");

  ok = stack_peek(&stack, &value);
  munit_assert_int(ok, ==, 1,
                   "Input: peek on stack [10, 20]\nExpected: peek returns 1 "
                   "(has value)\nActual:   peek returned 0");
  munit_assert_int(value, ==, 20,
                   "Input: stack contents [10, 20], then peek\nExpected top: "
                   "20\nActual top:   different value");

  stack_free(&stack);
});

munit_case(RUN, test_stack_grow_run, {
  IntStack stack;
  stack_init(&stack, 1);

  int ok;
  int value;

  ok = stack_push(&stack, 1);
  munit_assert_int(ok, ==, 1,
                   "Input: initial_capacity = 1, push 1\nExpected: push "
                   "returns 1\nActual:   push returned 0");

  ok = stack_push(&stack, 2);
  munit_assert_int(
      ok, ==, 1,
      "Input: stack capacity 1, push 1 then 2 (should grow)\nExpected: second "
      "push returns 1 (after resize)\nActual:   second push returned 0");

  ok = stack_push(&stack, 3);
  munit_assert_int(ok, ==, 1,
                   "Input: after resize, push 3 on stack [1, 2]\nExpected: "
                   "push returns 1\nActual:   push returned 0");

  ok = stack_pop(&stack, &value);
  munit_assert_int(ok, ==, 1,
                   "Input: stack [1, 2, 3], then pop\nExpected: pop returns "
                   "1\nActual:   pop returned 0");
  munit_assert_int(value, ==, 3,
                   "Input: stack [1, 2, 3], then pop\nExpected popped: 3 "
                   "(LIFO)\nActual popped:   different value");

  stack_free(&stack);
});

munit_case(SUBMIT, test_stack_pop_order_submit, {
  IntStack stack;
  stack_init(&stack, 2);

  int ok;
  int value;

  ok = stack_push(&stack, 100);
  ok &= stack_push(&stack, 200);
  ok &= stack_push(&stack, 300);
  munit_assert_int(
      ok, ==, 1,
      "Input: push 100, 200, 300 (with growth)\nExpected: all pushes succeed "
      "(1)\nActual:   at least one push returned 0");

  ok = stack_pop(&stack, &value);
  munit_assert_int(ok, ==, 1,
                   "Input: stack [100, 200, 300], then pop\nExpected: pop "
                   "returns 1\nActual:   pop returned 0");
  munit_assert_int(value, ==, 300,
                   "Input: stack [100, 200, 300], then first pop\nExpected "
                   "popped: 300\nActual popped:   different value");

  ok = stack_pop(&stack, &value);
  munit_assert_int(ok, ==, 1,
                   "Input: after popping 300, stack [100, 200], then "
                   "pop\nExpected: pop returns 1\nActual:   pop returned 0");
  munit_assert_int(value, ==, 200,
                   "Input: stack [100, 200], then second pop\nExpected popped: "
                   "200\nActual popped:   different value");

  ok = stack_pop(&stack, &value);
  munit_assert_int(ok, ==, 1,
                   "Input: after popping 300 and 200, stack [100], then "
                   "pop\nExpected: pop returns 1\nActual:   pop returned 0");
  munit_assert_int(value, ==, 100,
                   "Input: stack [100], then third pop\nExpected popped: "
                   "100\nActual popped:   different value");

  stack_free(&stack);
});

munit_case(SUBMIT, test_stack_empty_cases_submit, {
  IntStack stack;
  stack_init(&stack, 0);

  int ok;
  int value = 999;

  ok = stack_pop(&stack, &value);
  munit_assert_int(
      ok, ==, 0,
      "Input: empty stack created with initial_capacity 0, then pop\nExpected: "
      "pop returns 0 (empty)\nActual:   pop returned 1");

  ok = stack_peek(&stack, &value);
  munit_assert_int(
      ok, ==, 0,
      "Input: empty stack created with initial_capacity 0, then "
      "peek\nExpected: peek returns 0 (empty)\nActual:   peek returned 1");

  stack_free(&stack);
});

munit_case(SUBMIT, test_stack_free_and_reuse_submit, {
  IntStack stack;
  stack_init(&stack, 3);

  int ok;
  int value;

  ok = stack_push(&stack, 7);
  ok &= stack_push(&stack, 8);
  ok &= stack_push(&stack, 9);
  munit_assert_int(ok, ==, 1,
                   "Input: initial stack, push 7, 8, 9\nExpected: all pushes "
                   "succeed (1)\nActual:   at least one push returned 0");

  stack_free(&stack);

  stack_init(&stack, 1);
  ok = stack_push(&stack, 42);
  munit_assert_int(ok, ==, 1,
                   "Input: after free, re-init with capacity 1, then push "
                   "42\nExpected: push returns 1\nActual:   push returned 0");

  ok = stack_peek(&stack, &value);
  munit_assert_int(ok, ==, 1,
                   "Input: stack [42], then peek\nExpected: peek returns "
                   "1\nActual:   peek returned 0");
  munit_assert_int(value, ==, 42,
                   "Input: stack [42], then peek\nExpected top: 42\nActual "
                   "top:   different value");

  stack_free(&stack);
});

int main() {
  MunitTest tests[] = {
      munit_test("/run/stack_basic", test_stack_basic_run),
      // munit_test("/run/stack_grow", test_stack_grow_run),
      // munit_test("/submit/stack_order", test_stack_pop_order_submit),
      // munit_test("/submit/stack_empty", test_stack_empty_cases_submit),
      // munit_test("/submit/stack_free_reuse",
      // test_stack_free_and_reuse_submit),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("int_stack", tests);
  return munit_suite_main(&suite, NULL, 0, NULL);
}
