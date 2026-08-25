#include "snek_ms_vm.h"
#include "snek_obj.h"
#include <stdlib.h>

void vm_track_object(vm_t *vm, snek_object_t *obj) {
  if (vm == NULL || obj == NULL) {
    return;
  }
  stack_push(vm->objects, obj);
}

void vm_frame_push(vm_t *vm, frame_t *frame) {
  if (vm == NULL || frame == NULL) {
    return;
  }
  stack_push(vm->frames, frame);
}

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
  if (frame == NULL) {
    return;
  }
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
  // vm_free needs to walk vm->frames and frame_free each entry before calling
  // stack_free on the container.
  for (size_t i = 0; i < vm->frames->count; i++) {
    frame_free(vm->frames->data[i]);
  }
  stack_free(vm->frames);
  stack_free(vm->objects);
  free(vm);
}
