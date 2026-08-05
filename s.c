#include <stdint.h>
#include <stdio.h>

void concat_strings(char *str1, const char *str2) {
  int str1_len = 0;
  while (str1[str1_len] != '\0') {
    str1_len++;
  }

  int str2_len = 0;
  while (str2[str2_len] != '\0') {
    str2_len++;
  }

  for (int i = 0; i < str2_len; i++) {
    // printf("%c\n", str2[i]);
    str1[str1_len + i] = str2[i];
    // printf("%s\n", str1);
    // printf("i is at %d\n", i);
  }
  str1[str1_len + str2_len] = '\0';

  // printf("null byte of str1 %i\n", str1_len);
  // printf("null byte of str2 %i\n", str2_len);
  printf("%s\n", str1);
}

int main(void) {
  char first[50] = "Snek";
  char *second = "lang!";
  concat_strings(first, second);
  printf("%s\n", first);
}
