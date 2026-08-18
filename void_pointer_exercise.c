#include "void_pointer_exercise.h"

void snek_zero_out(void *ptr, snek_object_kind_t kind) {
  // ?
  if (kind == INTEGER) {
    ((snek_int_t *)ptr)->value = 0;
  } else if (kind == FLOAT) {
    ((snek_float_t *)ptr)->value = 0.0;
  } else if (kind == BOOL) {
    ((snek_bool_t *)ptr)->value = 0;
  }
}
