#include <stdio.h>

typedef struct MyStruct {
  int a;
  int b;
  int c;
  int d;
  int e;
  int f;
  int g;
  int h;
} my_struct;

int main(void) {
  my_struct points[3] = {
      {1, 2, 3, 4, 5, 6, 7, 8},
      {10, 20, 30, 40, 50, 60, 70, 80},
  };
  printf("points[1].x = %d, points[1].y = %d, points[1].z = %d\n", points[1].a,
         points[1].b, points[1].c);

  int *points_ptr = (int *)points;
  for (int i = 0; i < 16; i++) {
    printf("points_start[%d] = %d\n", i, points_ptr[i]);
  }

  int numbers[5] = {1, 2, 3, 4, 5};
  // equivalent to int *numbers_ptr = &numbers[0];
  int *numbers_ptr = numbers;
  printf("array ptr %d\n", *numbers_ptr);

  int second_value = numbers[2];

  // this is the same as *(numbers + 2);
  printf("second %d\n", second_value);

  int *p = numbers + 2;
  int v = *p;
  printf("v %d\n", v);

  // out of bound
  printf("%d\n", numbers[10]);
  return 0;
}
