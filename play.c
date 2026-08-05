#include <stdint.h>
#include <stdio.h>

void print_bits(uint8_t value) {
  //
}

int streak_reward(int days) {
  // your code here
  int base = 10;
  int r = 0;
  for (int i = 1; i <= days; i++) {
    if (i % 7 == 0) {
      r += 20;
    } else if (i % 3 == 0) {
      r += 5;
    }
  }

  return days * base + r;
}

// int base = 10;
// int bonus = 0;
// for (int i = 1; i <= days; i++) {
//   if (i % 7 == 0) {
//     bonus += 20;
//   } else if (i % 3 == 0) {
//     bonus += 5;
//   }
// }

int main(void) {
  int r = streak_reward(5);
  printf("%d\n", r);
  int j = streak_reward(7);
  printf("%d\n", j);
  // printf("45");
  // print_bits(45);
  // printf("\n");
  // printf("1 << 6");
  // print_bits(1 << 6);
  // printf("\n");
  return 0;
}
