#include "snekstack_hetro.h"
#include "vendor/bootlib.h"
#include "vendor/munit/munit.h"

munit_case(RUN, heterogenous_stack, {
  stack_t *s = stack_new(2);
  assert_ptr_not_null(s, "Must allocate a new stack");

  scary_double_push(s);
  assert_int(s->count, ==, 2, "Should have two items in the stack");

  int value = (int)s->data[0];
  assert_int(value, ==, 1337, "Zero item should be 1337");

  int *pointer = s->data[1];
  assert_int(*pointer, ==, 1024, "Top item should be 1024");

  free(pointer);
  stack_free(s);
});

int main() {
  MunitTest tests[] = {
      munit_test("/heterogenous_stack", heterogenous_stack),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("snekstack", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
