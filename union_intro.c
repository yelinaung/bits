#include <stdio.h>

typedef union AgeOrName {
  int age;
  char *name;
} age_or_name_t;

int main(void) {
  age_or_name_t lane = {.age = 29};
  printf("size of lane: %zu\n", sizeof(age_or_name_t));
  printf("age: %d\n", lane.age);
  // this will crash
  // printf("name: %s\n", lane.name);

  age_or_name_t lane2 = {.name = "John"};
  printf("size of lane2: %zu\n", sizeof(age_or_name_t));
  printf("age: %d\n", lane2.age);
  printf("name: %s\n", lane2.name);
  return 0;
}
