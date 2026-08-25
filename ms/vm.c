#include "vm.h"
#include "snekobject.h"
#include "stack.h"
#include <stdbool.h>

void vm_collect_garbage(vm_t *vm) {
  mark(vm);
  trace(vm);
  sweep(vm);
}

void sweep(vm_t *vm) {
  for (size_t i = 0; i < vm->objects->count; i++) {
    snek_object_t *obj = vm->objects->data[i];
    if (obj->is_marked == true) {
      obj->is_marked = false;
    } else {
      //  free the object and set the data at that position in the stack to NULL
      snek_object_free(obj);
      vm->objects->data[i] = NULL;
    }
  }
  stack_remove_nulls(vm->objects);
}

void trace(vm_t *vm) {
  stack_t *gray_objects = stack_new(8);
  if (gray_objects == NULL) {
    return;
  }
  for (size_t i = 0; i < vm->objects->count; i++) {
    snek_object_t *obj = vm->objects->data[i];
    if (obj->is_marked) {
      stack_push(gray_objects, obj);
    }
  }
  while (gray_objects->count > 0) {
    snek_object_t *obj = stack_pop(gray_objects);
    trace_blacken_object(gray_objects, obj);
  }
  stack_free(gray_objects);
}

void trace_blacken_object(stack_t *gray_objects, snek_object_t *obj) {
  if (gray_objects == NULL || obj == NULL) {
    return;
  }

  switch (obj->kind) {
  case INTEGER:
  case FLOAT:
  case STRING:
    break;
  case VECTOR3:
    trace_mark_object(gray_objects, obj->data.v_vector3.x);
    trace_mark_object(gray_objects, obj->data.v_vector3.y);
    trace_mark_object(gray_objects, obj->data.v_vector3.z);
    break;
  case ARRAY:
    for (size_t i = 0; i < obj->data.v_array.size; i++) {
      trace_mark_object(gray_objects, obj->data.v_array.elements[i]);
    }
    break;
  default:
    break;
  }
}

void trace_mark_object(stack_t *gray_objects, snek_object_t *obj) {
  if (obj == NULL || obj->is_marked == true) {
    return;
  }
  obj->is_marked = true;
  stack_push(gray_objects, obj);
}

void mark(vm_t *vm) {
  // Iterate over each frame in the VM
  for (size_t i = 0; i < vm->frames->count; i++) {
    frame_t *f = vm->frames->data[i];
    // Iterate over each references object in each frame
    for (size_t j = 0; j < f->references->count; j++) {
      // Mark the objects as is_marked = true
      snek_object_t *obj = f->references->data[j];
      obj->is_marked = true;
    }
  }
}

void frame_reference_object(frame_t *frame, snek_object_t *obj) {
  // push the object onto the stack of references for the current frame.
  stack_push(frame->references, obj);
}

void vm_free(vm_t *vm) {
  for (size_t i = 0; i < vm->frames->count; i++) {
    frame_free(vm->frames->data[i]);
  }
  stack_free(vm->frames);
  for (size_t i = 0; i < vm->objects->count; i++) {
    snek_object_free(vm->objects->data[i]);
  }
  stack_free(vm->objects);
  free(vm);
}

// don't touch below this line

vm_t *vm_new(void) {
  vm_t *vm = malloc(sizeof(vm_t));
  if (vm == NULL) {
    return NULL;
  }

  vm->frames = stack_new(8);
  vm->objects = stack_new(8);
  return vm;
}

void vm_track_object(vm_t *vm, snek_object_t *obj) {
  stack_push(vm->objects, obj);
}

void vm_frame_push(vm_t *vm, frame_t *frame) { stack_push(vm->frames, frame); }

frame_t *vm_new_frame(vm_t *vm) {
  frame_t *frame = malloc(sizeof(frame_t));
  frame->references = stack_new(8);

  vm_frame_push(vm, frame);
  return frame;
}

void frame_free(frame_t *frame) {
  stack_free(frame->references);
  free(frame);
}

frame_t *vm_frame_pop(vm_t *vm) { return stack_pop(vm->frames); }
