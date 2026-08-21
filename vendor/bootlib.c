#define BOOTLIB_INTERNAL
#include "bootlib.h"

/* One record per live allocation. free() is handed only a pointer, so the size
 * has to be looked up to know how much to subtract from the byte total.
 *
 * The records are kept in a side list rather than in a header prepended to
 * each block. A prepended header would mean returning an interior pointer,
 * which blunts AddressSanitizer's view of where each block really begins.
 * Lookup is linear, which is fine at test scale. */
typedef struct BootRecord {
  void *ptr;
  size_t size;
  struct BootRecord *next;
} boot_record_t;

static boot_record_t *boot_records = NULL;
static size_t boot_outstanding = 0;
static size_t boot_bytes = 0;

void *boot_malloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    return NULL;
  }

  boot_record_t *rec = malloc(sizeof(boot_record_t));
  if (rec == NULL) {
    /* Cannot track it, so do not count it either. The allocation itself is
     * still valid and is returned to the caller. */
    return ptr;
  }

  rec->ptr = ptr;
  rec->size = size;
  rec->next = boot_records;
  boot_records = rec;

  boot_outstanding++;
  boot_bytes += size;
  return ptr;
}

void boot_free(void *ptr) {
  /* free(NULL) is a no-op that was never counted, so it must not decrement. */
  if (ptr == NULL) {
    return;
  }

  boot_record_t **link = &boot_records;
  while (*link != NULL) {
    if ((*link)->ptr == ptr) {
      boot_record_t *dead = *link;
      *link = dead->next;
      boot_outstanding--;
      boot_bytes -= dead->size;
      free(dead);
      break;
    }
    link = &(*link)->next;
  }

  /* An untracked pointer, from a file that does not include this header, is
   * still released but leaves the counters alone. */
  free(ptr);
}

size_t boot_alloc_count(void) { return boot_outstanding; }

size_t boot_alloc_size(void) { return boot_bytes; }

bool boot_all_freed(void) { return boot_outstanding == 0; }

void boot_reset(void) {
  boot_record_t *rec = boot_records;
  while (rec != NULL) {
    boot_record_t *next = rec->next;
    free(rec);
    rec = next;
  }
  boot_records = NULL;
  boot_outstanding = 0;
  boot_bytes = 0;
}
