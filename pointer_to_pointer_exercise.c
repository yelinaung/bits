#include "pointer_to_pointer_exercise.h"
#include <stdlib.h>

void allocate_int(int **pointer_pointer, int value) {
  // ?
  // Allocate memory for a single integer on the heap,
  // and save its address into a new pointer.
  int *v = malloc(sizeof(int));
  if (v == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }
  // Update the pointer that pointer_pointer is pointing at
  // to hold the newly allocated memory address.
  // i.e. dereferece the pointer_pointer to v
  *pointer_pointer = v;

  // Set the raw value of the integer that pointer_pointer now points to
  // to the value passed in.
  **pointer_pointer = value;
}
