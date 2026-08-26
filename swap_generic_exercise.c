#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bootlib.h"

void swap(void *vp1, void *vp2, size_t size) {
  if (size <= 0)
    exit(1);

  // Allocate memory for a temporary buffer to store the data using malloc.
  void *v = malloc(size);

  // If the allocation fails (if it's NULL) return immediately
  if (v == NULL)
    exit(1);

  // Use memcpy to shuffle the data around.
  // NOTE: memcpy(dest, source)
  memcpy(v, vp1, size);
  memcpy(vp1, vp2, size);
  memcpy(vp2, v, size);

  // Don't forget to free the temporary buffer
  free(v);
}

// typedef struct CoffeeShop {
//   uint64_t quality;
//   uint64_t taste;
//   uint64_t branding;
// } coffee_shop_t;
//
// int main(void) {
//   int i1 = 1234;
//   int i2 = 5678;
//   swap(&i1, &i2, sizeof(int));
//
//   char *s1 = "dax";
//   char *s2 = "adam";
//   swap(&s1, &s2, sizeof(char *));
//
//   coffee_shop_t sbucks = {2, 3, 4};
//   coffee_shop_t terminalshop = {10, 10, 10};
//
//   swap(&sbucks, &terminalshop, sizeof(coffee_shop_t));
// }
