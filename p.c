#include <stdio.h>

typedef struct CodeFile {
  int lines;
  int filetype;
} codefile_t;

codefile_t change_filetype(codefile_t *f, int new_filetype) {
  codefile_t new_f = *f;
  new_f.filetype = new_filetype;
  return new_f;
}

int main(void) {
  // codefile_t original;
  // original.lines = 100;
  // original.filetype = 1;
  // codefile_t result = change_filetype(&original, 2);
  // printf("Filetype should now be %d\n", result.filetype);
  //
  // codefile_t *ptr_cf = &original;
  // printf("ptr struct %d\n", ptr_cf->lines);

  int x = 5;
  printf("x = %d\n", x);
  printf("x address (&x) = %p\n", (void *)&x);
  printf("\n");

  int y = x;
  printf("y = %d\n", y);
  printf("y address (&y) = %p\n", (void *)&y);
  printf("\n");

  x = 7;
  printf("x after reassign = %d\n", x);
  printf("x address (&x) = %p\n", (void *)&x);
  printf("\n");

  int *x_pointer = &x;
  printf("x_pointer value (address that it holds)\t = %p\n", (void *)x_pointer);
  printf("x_pointer's own address (&x_pointer)\t = %p\n", (void *)&x_pointer);
  printf("\n");

  int z = *x_pointer;
  printf("z = %d\n", z);
  printf("\n");

  /*
  - & goes from a value → its address.
  - * goes from an address → the value stored there
  */
  // int a = 100;
  // int b = 200;
  // printf("a = %d, b = %d\n", a, b);
  // printf("addresses a = %p, b = %p\n", &a, &b);
  // printf("\n");
  //
  // printf("dereference values a = %d, b = %d\n", *&a, *&b);
  //
  // int *a_ptr = &a;
  // int *b_ptr = &b;
  // printf("addresses a = %p, b = %p\n", (void*) a_ptr, (void*) b_ptr);
  // printf("values a = %d, b = %d\n", *a_ptr, *b_ptr);
  //
  // *a_ptr = b;
  // printf("values a = %d, b = %d\n", *a_ptr, *b_ptr);
  //
  int a = 100;
  int b = 200;
  printf("[values] a = %d, b = %d\n", a, b);

  // NOTE:
  // & works on the objects that have a memory values
  // &x           // ✅ x is a variable, it lives somewhere
  // &5           // ❌ error — 5 is a literal, it has no address
  // &(a + b)     // ❌ error — the sum is a temporary, no address
  printf("[pointers] a = %p, b = %p\n", &a, &b);

  // getting back the value
  printf("[getting back values via pointers] a = %d, b = %d\n", *&a, *&b);

  // declaration
  // * is part of the "type"
  int *a_ptr = &a;
  int *b_ptr = &b;
  printf("a = %p, b = %p\n", (void *)a_ptr, (void *)b_ptr);

  // NOTE: statement
  // * is dereferencing - go to the address [getting back values via pointers]
  // that a_ptr holds and set the new
  // value
  *a_ptr = 300;
  printf("a = %d, b = %d\n", a, b);

  return 0;
}
