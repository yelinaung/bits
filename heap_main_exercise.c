#include "heap_main_exercise.h"
#include <stdio.h>
#include <stdlib.h>

char *get_full_greeting(char *greeting, char *name, int size) {
  if (size <= 0)
    return NULL;
  size_t n = (size_t)size;
  char *full_greeting = malloc(n);
  if (full_greeting == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1); // Exit if allocation fails
  }
  snprintf(full_greeting, n, "%s %s", greeting, name);
  return full_greeting;
}
