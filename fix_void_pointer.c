#include "fix_void_pointer.h"

int read_int(void *value) { return *(int *)value; }

double read_double(void *value) { return *(double *)value; }

void add_to_int(void *value, int amount) {
  // NOTE: amount is a parameter, a local copy of the number 3, living in this
  // function's frame. explicit casts from void * to the correct pointer type
  int *v = (int *)value;
  // dereference that typed pointer to read or update the value.
  // whatever sits on the left of += is what gets written.
  *v += amount;
}

void add_to_double(void *value, double amount) { *(double *)value += amount; }

void shift_char(void *value, int amount) { *(char *)value += amount; }
