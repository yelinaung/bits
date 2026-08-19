void swap_strings(char **a, char **b) {
  char *temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

// int main(void) {
//   char *a = "terminal.shop";
//   char *b = "ssh";
//
//   swap_strings(&a, &b);
//
//   // char *x = "Hello";
//   // char *y = "Goodbye";
//   //
//   // swap_strings(&x, &y);
// }
