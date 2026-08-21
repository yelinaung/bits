#include "snekstack.h"
#include <stdlib.h>

#include "bootlib.h"

stack_t *stack_new(size_t capacity) {
  // Allocate memory for a new Stack struct on the heap.
  stack_t *s = malloc(sizeof(stack_t));
  // If allocation fails, return NULL.
  if (s == NULL) {
    return NULL;
  }
  // Initialize the count to 0.
  s->count = 0;
  // Initialize the capacity to the given value.
  s->capacity = capacity;
  // Initialize the data by allocating enough memory for capacity number of void
  // * pointers.
  void **data = malloc(capacity * sizeof(s->data));
  // If the data allocation fails, free the Stack struct and return NULL.
  if (data == NULL) {
    free(s);
    return NULL;
  }
  s->data = data;

  return s;
}
