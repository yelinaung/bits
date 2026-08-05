#include <stdio.h>
#include <string.h>

typedef struct {
  size_t length;
  char buffer[64];
} TextBuffer;

int smart_append(TextBuffer *dest, const char *src) {
  // ?
  // either the dest or src input is NULL, return 1
  if (dest == NULL || src == NULL) {
    return 1;
  }

  const int max_buffer_size = 64;
  // Get the length of the src string using strlen.
  int src_len = strlen(src);

  // Calculate the remaining space in the dest buffer.
  // Notice that it stores its own length.
  // The 64-byte buffer can hold 63 characters plus the null terminator.
  int remaining_buf = max_buffer_size - dest->length - 1;
  // If the src string is larger than the remaining space:
  if (src_len > remaining_buf) {
    // Copy as much of the src string as possible to the dest buffer using
    // strncat.
    strncat(dest->buffer, src, remaining_buf);

    // Update the dest buffer length to the max size, accounting for the null
    // terminator.
    dest->length = strlen(dest->buffer);

    // Return 1 (failure) to indicate the full append wasn't possible.
    return 1;
  } else {
    // Otherwise, if there's enough space:
    // Append the entire src string to the dest buffer using strcat
    strcat(dest->buffer, src);
    // Update the dest buffer length
    dest->length = strlen(dest->buffer);

    // Return 0 (success) to indicate the full append was possible
    return 0;
  }
  return -1;
}

int main(void) {
  TextBuffer dest;
  strcpy(dest.buffer, "");
  dest.length = 0;
  const char *src = "Hello";
  int result = smart_append(&dest, src);
  printf("result %d\n", result);
  printf("dest buffer %s\n", dest.buffer);
  printf("dest lenght %zu\n", dest.length);

  printf("\n");

  TextBuffer dest2;
  const char *src2 = NULL;
  int result2 = smart_append(&dest2, src2);
  printf("result2 %d\n", result2);

  printf("\n");

  TextBuffer dest3;
  strcpy(dest3.buffer,
         "This is a very long string that will fill up the entire buffer.");
  dest3.length = 63;
  const char *src3 = " Extra";
  int result3 = smart_append(&dest3, src3);
  printf("result3 %d\n", result3); // should be 1 for unsuccessful append
  printf("dest3 buffer %s\n", dest3.buffer);
  printf("dest3 lenght %zu\n", dest3.length); // Length should remain 63

  return 0;
}
