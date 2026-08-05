#include <stdint.h>
#include <stdio.h>

void f1(int v) { v = v + 1; }

void f2(int *v) { *v = *v + 1; }

int main(void) {
  int original = 10;
  f1(original);
  printf("original after f1 %d\n", original);

  f2(&original);
  printf("original after f2 %d\n", original);
  return 0;
}
