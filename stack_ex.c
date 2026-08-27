#include "stack_ex.h"
#include <stdio.h>
#include <stdlib.h>

// Prepares a stack the caller already owns, so pass the address of an IntStack
// you declared. Every stack that reaches this function needs a matching
// stack_free. A capacity of 0 or less, and a failed allocation, both leave an
// empty stack that stack_push can still grow.
void stack_init(IntStack *stack, int initial_capacity) {
  stack->size = 0;
  if (initial_capacity <= 0) {
    stack->data = NULL;
    stack->capacity = 0;
    return;
  }
  int *newInts = malloc(sizeof(int) * (size_t)initial_capacity);
  if (newInts == NULL) {
    stack->data = NULL;
    stack->capacity = 0;
    return;
  }
  stack->capacity = initial_capacity;
  stack->data = newInts;
}

// Adds value to the top, growing the array when the stack is full.
// Returns 1 on success, 0 when the allocation fails.
int stack_push(IntStack *stack, int value) {
  if (stack->size == stack->capacity) {
    // Double the capacity. An empty stack starts at 4.
    if (stack->capacity == 0) {
      stack->capacity += 4;
    } else {
      stack->capacity *= 2;
    }

    stack->data = realloc(stack->data, (size_t)stack->capacity * sizeof(int));
    if (stack->data == NULL) {
      return 0;
    }
  }
  // size counts the elements, so it also indexes the next free slot.
  stack->data[stack->size] = value;
  stack->size++;
  return 1;
}

// Removes the top element and writes it through out_value, which may be NULL.
// Returns 1 if the stack held an element, 0 if it was empty.
int stack_pop(IntStack *stack, int *out_value) {
  if (stack->size == 0) {
    return 0;
  }
  // The decrement makes size index the element being removed.
  stack->size--;
  if (out_value != NULL) {
    *out_value = stack->data[stack->size];
  }
  return 1;
}

// Reads the top element without removing it. size stays put, so the top sits
// at size - 1.
// Returns 1 if the stack held an element, 0 if it was empty.
int stack_peek(const IntStack *stack, int *out_value) {
  if (stack->size == 0) {
    return 0;
  }

  if (out_value != NULL) {
    *out_value = stack->data[stack->size - 1];
  }
  // The return value reports the stack, not the write. A NULL out_value still
  // counts as success.
  return 1;
}

// Releases the array and clears the fields, so the stack is safe to reuse
// through stack_init or to free a second time.
void stack_free(IntStack *stack) {
  if (stack->data != NULL) {
    free(stack->data);
  }
  stack->data = NULL;
  stack->capacity = 0;
  stack->size = 0;
}

// int main(void) {
//   // Basic push/pop
//   IntStack stack;
//   stack_init(&stack, 2);
//
//   stack_push(&stack, 10);
//   stack_push(&stack, 20);
//
//   int value;
//   stack_pop(&stack, &value); // value == 20
//   stack_pop(&stack, &value); // value == 10
//   //
//   stack_free(&stack);
//   //
//   // automatic growth
//   IntStack stack1;
//   stack_init(&stack1, 1);
//
//   stack_push(&stack1, 1);
//   stack_push(&stack1, 2); // should trigger a resize
//   stack_push(&stack1, 3);
//
//   // Now size == 3 and all values are stored in LIFO order
//   stack_free(&stack1);
// }
