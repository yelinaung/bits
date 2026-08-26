#include <stdio.h>

int main(void) {
  int v1 = 1;
  // int v2 = 2;
  // int checkpoint = 0;

  int *ptr_v1 = &v1;
  int **ptr_ptr_v1 = &ptr_v1;

  // Each * steps one rung down toward the value, each & one rung up.
  printf("v1           = %d\n", v1);
  printf("&v1          = %p\n", (void *)&v1);
  // getting back the original value
  printf("*&v1         = %d  (& and * cancel)\n", *&v1);

  printf("ptr_v1       = %p  (== &v1)\n", (void *)ptr_v1);
  printf("*ptr_v1      = %d\n", *ptr_v1);

  printf("ptr_ptr_v1   = %p  (== &ptr_v1)\n", (void *)ptr_ptr_v1);
  printf("*ptr_ptr_v1  = %p  (== ptr_v1 == &v1)\n", (void *)*ptr_ptr_v1);
  printf("**ptr_ptr_v1 = %d  (all the way down)\n", **ptr_ptr_v1);

  // NOTE: pointer pointer
  // &  adds a level    (int *  →  int **)
  // *  removes a level (int ** →  int *)
  // for int **ptr_ptr_v1
  // ┌───────────────────┬─────────┬───────┐
  // │    Expression     │  Type   │ Stars │
  // ├───────────────────┼─────────┼───────┤
  // │ &pointer_pointer  │ int *** │ 3     │
  // ├───────────────────┼─────────┼───────┤
  // │ pointer_pointer   │ int **  │ 2     │ <-- we are here
  // ├───────────────────┼─────────┼───────┤
  // │ *pointer_pointer  │ int *   │ 1     │
  // ├───────────────────┼─────────┼───────┤
  // │ **pointer_pointer │ int     │ 0     │
  // └───────────────────┴─────────┴───────┘
  return 0;
}
