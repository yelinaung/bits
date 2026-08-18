void swap_ints(int *a, int *b) {
  int temp;
  // dereference a and assign its value to temp
  temp = *a;
  // dereference b and assign its value to a pointer. what's b at this point
  *a = *b;
  // assign temp back to b ?
  *b = temp;
}
