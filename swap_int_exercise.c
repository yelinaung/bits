void swap_ints(int *a, int *b) {
  int temp;
  // dereference a and assign its value to temp
  temp = *a;
  // changes WHAT a points at, the caller's int
  // in this case, a value points at b value (by dereferencing b)
  *a = *b;
  // assign temp back to b
  *b = temp;
}
