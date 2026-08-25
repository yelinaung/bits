#include "bootlib.h"
#include "munit.h"
#include "sneknew.h"
#include "vm.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

munit_case(RUN, test_reference_object, {
  vm_t *vm = vm_new();
  new_snek_integer(vm, 5);
  new_snek_string(vm, "hello");
  vm_free(vm);
  assert(boot_all_freed());
});

munit_case(RUN, test_array_freed, {
  vm_t *vm = vm_new();
  new_snek_array(vm, 3);
  vm_free(vm);
  assert(boot_all_freed());
});

munit_case(SUBMIT, test_frames_are_freed, {
  vm_t *vm = vm_new();
  vm_new_frame(vm);
  vm_new_frame(vm);
  vm_free(vm);
  assert(boot_all_freed());
});

int main() {
  MunitTest tests[] = {
      munit_test("/test_reference_object", test_reference_object),
      munit_test("/test_array_freed", test_array_freed),
      munit_test("/test_frames_are_freed", test_frames_are_freed),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("mark-and-sweep", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
