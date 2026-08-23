#include "snekstack.h"
#include <stdlib.h>

void scary_double_push(stack_t *s) {
  // Push the value 1337 directly onto the stack using the stack_push function.
  // cast the value to a void *.
  stack_push(s, (void *)1337);

  // allocate memory for a new int on the heap
  int *i = malloc(sizeof(int));
  if (i == NULL) {
    return;
  }
  // set the value to which the address points to 1024
  *i = 1024;

  // Push the int pointer onto the stack using the stack_push function
  stack_push(s, i);
}
