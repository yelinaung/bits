#include "snekstack_multiple_types.h"
#include "stdlib.h"
#include <stdlib.h>
#include <string.h>

void stack_push_multiple_types(stack_t *s) {
  // Allocate memory on the heap for a float and
  // set the value to which it's pointed to 3.14.
  float *f = malloc(sizeof(float));
  if (f == NULL) {
    return;
  }
  *f = 3.14f;
  // Push the float * onto the stack using stack_push.
  stack_push(s, f);
  // Create a char * constant that points to the value:
  const char *c = "Sneklang is blazingly slow!";
  // Get the length of the string for memory allocation.
  size_t c_size = strlen(c) + 1;
  // Allocate memory for a char *.
  char *c2 = malloc(c_size);
  if (c2 == NULL) {
    return;
  }
  // Copy the value to the memory.
  strcpy(c2, c);

  // Push the pointer to the memory onto the stack using stack_push
  stack_push(s, c2);
}
