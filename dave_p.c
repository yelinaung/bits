#include <stdio.h>

int main(void) {

  int color = 53281;
  int *p_color = &color;
  printf("color %d\n", color);
  printf("color %pd\n", p_color);

  *p_color = 9988;
  printf("color %d\n", color);
  printf("color %pd\n", p_color);

  int txt[] = {1, 2, 3, 4, 5};
  int *p_txt = txt;
  p_txt++;

  return 0;
}
