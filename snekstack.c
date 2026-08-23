#include "snekstack.h"
#include <stdlib.h>
#include <string.h>

#include "vendor/bootlib.h"

// MY IMPLEMENTATION
// stack_t *stack_new(size_t capacity) {
//   // Allocate memory for a new Stack struct on the heap.
//   stack_t *s = malloc(sizeof(stack_t));
//   // If allocation fails, return NULL.
//   if (s == NULL) {
//     return NULL;
//   }
//   // Initialize the count to 0.
//   s->count = 0;
//   // Initialize the capacity to the given value.
//   s->capacity = capacity;
//   // Initialize the data by allocating enough memory for capacity number of
//   void
//   // * pointers.
//   // NOTE: sizeof(void *)  —> one element
//   void **data = malloc(capacity * sizeof(*s->data));
//   // If the data allocation fails, free the Stack struct and return NULL.
//   if (data == NULL) {
//     free(s);
//     return NULL;
//   }
//   s->data = data;
//
//   return s;
// }
//

// void stack_push(stack_t *stack, void *obj) {
//   // If the stack's count is equal to the stack's capacity
//   if (stack->count == stack->capacity) {
//     // Double the stack's capacity
//     stack->capacity = stack->capacity * 2;
//
//     // Reallocate enough memory for the stack's data using the new capacity
//     void **data = realloc(stack->data, stack->capacity * sizeof(void *));
//
//     // If realloc fails, set the stack's capacity back and return from the
//     // function The old data is still valid.
//     if (data == NULL) {
//       stack->capacity = stack->capacity / 2;
//       return;
//     }
//     // If it succeeds, update the stack's data field to point to the new
//     memory stack->data = data;
//   }
//   // Add the new object to the top of the stack (the count-th element in the
//   // array)
//   stack->data[stack->count] = obj;
//
//   // Increment the stack's count
//   stack->count += 1;
// }

void *stack_pop(stack_t *stack) {
  // If the stack has no elements, return NULL.
  if (stack->count == 0) {
    return NULL;
  }
  // Decrement the stack's count.
  stack->count -= 1;

  // Return the top element of the stack (the count-th element in the array).
  return stack->data[stack->count];
}

void stack_free(stack_t *stack) {
  if (stack == NULL) {
    return;
  }
  if (stack->data != NULL) {
    free(stack->data);
  }
  free(stack);
}

// BOOT.DEV ONE
void stack_push(stack_t *stack, void *obj) {
  if (stack->count == stack->capacity) {
    stack->capacity *= 2;
    void **temp = realloc(stack->data, stack->capacity * sizeof(void *));
    if (temp == NULL) {
      stack->capacity /= 2;

      exit(1);
    }
    stack->data = temp;
  }
  stack->data[stack->count] = obj;
  stack->count++;
  return;
}

stack_t *stack_new(size_t capacity) {
  stack_t *stack = malloc(sizeof(stack_t));
  if (stack == NULL) {
    return NULL;
  }

  stack->count = 0;
  stack->capacity = capacity;
  stack->data = malloc(stack->capacity * sizeof(void *));
  if (stack->data == NULL) {
    free(stack);
    return NULL;
  }

  return stack;
}
