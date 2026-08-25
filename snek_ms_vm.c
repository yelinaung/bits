#include "snek_ms_vm.h"
#include <stdlib.h>

void vm_frame_push(vm_t *vm, frame_t *frame) { stack_push(vm->frames, frame); }

frame_t *vm_new_frame(vm_t *vm) {
  frame_t *f = malloc(sizeof(frame_t));
  if (f == NULL) {
    return NULL;
  }
  f->references = stack_new(8);
  vm_frame_push(vm, f);
  return f;
}

void frame_free(frame_t *frame) {
  stack_free(frame->references);
  free(frame);
}

vm_t *vm_new(void) {
  vm_t *v = malloc(sizeof(vm_t));
  if (v == NULL) {
    return NULL;
  }
  v->frames = stack_new(8);
  v->objects = stack_new(8);
  return v;
}

void vm_free(vm_t *vm) {
  stack_free(vm->frames);
  stack_free(vm->objects);
  free(vm);
}
