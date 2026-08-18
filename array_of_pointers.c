#include <stdlib.h>
int main(void) {
  char **str_array = malloc(sizeof(char *) * 3);
  str_array[0] = "foo";
  str_array[1] = "bar";
  str_array[2] = "baz";
  return 0;
}
