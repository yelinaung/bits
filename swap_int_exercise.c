void swap_ints(int *a, int *b) {
  // Every assignment below writes through a pointer, so it lands in the
  // caller's int. The pointers a and b themselves never change.
  int temp;
  temp = *a; // save the caller's int that a points at, by dereferencing a
  /*
   * *a = *b;
   * ^     ^
   * |     read the int b points at
   * write into the int a points at
   *
   */
  *a = *b;   // overwrite it with the caller's int that b points at
  *b = temp; // write the saved value into b's int
  /*
    before:   x: 3        y: 7
              a ─┘        b ─┘
    temp  = *a;     read x  → temp = 3
    *a    = *b;     read y (7), write into x   →  x: 7   y: 7
    *b    = temp;   write 3 into y             →  x: 7   y: 3
  */
}
