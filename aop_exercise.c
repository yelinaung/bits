#include "aop_exercise.h"
#include <stdlib.h>

token_t **create_token_pointer_array(token_t *tokens, size_t count) {
  token_t **token_pointers = malloc(count * sizeof(token_t *));
  if (token_pointers == NULL) {
    exit(1);
  }
  for (size_t i = 0; i < count; ++i) {
    // Update the create_token_pointer_array's loop
    // to allocate new space for each token on the heap.
    token_t *t = malloc(sizeof(token_t));
    if (t == NULL) {
      exit(1);
    }
    // Store the address of the new space in the array instead of the stack
    // address.
    token_pointers[i] = t;
    // Copy the members of the input struct into the newly allocated one.
    token_pointers[i]->line = tokens[i].line;
    token_pointers[i]->column = tokens[i].column;
    token_pointers[i]->literal = tokens[i].literal;
  }
  return token_pointers;
}
