#pragma once

#include <stddef.h>

typedef struct {
  int *data;
  int capacity;
  int size;
} IntStack;

void stack_init(IntStack *stack, int initial_capacity);
int stack_push(IntStack *stack, int value);
int stack_pop(IntStack *stack, int *out_value);
int stack_peek(const IntStack *stack, int *out_value);
void stack_free(IntStack *stack);
