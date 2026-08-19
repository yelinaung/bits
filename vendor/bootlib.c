#define BOOTLIB_INTERNAL
#include "bootlib.h"

/* Outstanding allocations. One counter for the whole program, so the header
 * only declares and this file defines: a static in the header would give each
 * translation unit its own count and the assertions would never agree. */
static size_t boot_outstanding = 0;

void *boot_malloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr != NULL) {
    boot_outstanding++;
  }
  return ptr;
}

void boot_free(void *ptr) {
  /* free(NULL) is a no-op and was never counted, so it must not decrement. */
  if (ptr != NULL) {
    boot_outstanding--;
  }
  free(ptr);
}

size_t boot_alloc_count(void) { return boot_outstanding; }

bool boot_all_freed(void) { return boot_outstanding == 0; }

void boot_reset(void) { boot_outstanding = 0; }
