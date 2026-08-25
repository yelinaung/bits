#include "assert.h"
#include "snek_ms_vm.h"
#include "snek_obj.h"
#include "vendor/bootlib.h"
#include "vendor/munit/munit.h"

munit_case(RUN, test_vm_new, {
  vm_t *vm = vm_new();
  vm_new_frame(vm);
  assert_int(vm->frames->count, ==, 1, "frame was pushed");
  vm_free(vm);
});

munit_case(RUN, test_vm_new_frame, {
  vm_t *vm = vm_new();
  frame_t *frame = vm_new_frame(vm);
  assert_ptr(frame->references, !=, NULL,
             "frame->references must be allocated");
  assert_int(frame->references->count, ==, 0,
             "references stack should start empty");
  assert(frame->references->capacity >
         0); // references stack must have capacity > 0
  assert_ptr(frame->references->data, !=, NULL,
             "references stack backing array must be allocated");
  vm_free(vm);
});

munit_case(RUN, test_frames_are_freed, {
  vm_t *vm = vm_new();
  vm_new_frame(vm);
  vm_free(vm);
  assert(boot_all_freed());
});

int main() {
  MunitTest tests[] = {
      munit_test("/test_vm_new", test_vm_new),
      munit_test("/test_vm_new_frame", test_vm_new_frame),
      munit_test("/test_frames_are_freed", test_frames_are_freed),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("mark-and-sweep", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
