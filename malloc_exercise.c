#include "malloc_exercise.h"
#include <stdio.h>
#include <stdlib.h>

int *allocate_scalar_array(int size, int multiplier) {
  if (size <= 0)
    return NULL;
  size_t n = sizeof(int) * (size_t)size;
  int *arr = malloc(n);

  if (arr == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }
  for (int i = 0; i < size; i++) {
    arr[i] = i * multiplier;
  }
  return arr;
}
