#include <stdio.h>

typedef enum DATA_TYPE { INT, FLOAT } data_type_t;

void print_value(void *ptr, data_type_t type) {
  if (type == INT) {
    printf("value: %d\n", *(int *)ptr);
  } else if (type == FLOAT) {
    printf("value: %f\n", *(float *)ptr);
  }
}

int main(void) {
  int n = 42;
  void *generic_ptr = &n;
  // This doesn't work
  // printf("Value of number: %d\n", *generic_ptr);

  // This works: Cast to appropriate type before dereferencing
  printf("Value of number: %d\n", *(int *)generic_ptr);

  int num = 42;
  print_value(&num, INT);

  float f = 42.314;
  print_value(&f, FLOAT);
}
