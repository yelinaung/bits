#define BOOTLIB_INTERNAL
#include "bootlib.h"

/* Belt and braces: if bootlib.h ever arrives by -include, before the
 * BOOTLIB_INTERNAL define above takes effect, the wrappers below would call
 * themselves. Undefining here keeps this file on the real allocator. */
#undef malloc
#undef realloc
#undef free

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
static size_t boot_reallocs = 0;
static size_t boot_last_realloc = 0;

/* Unlink and return the record for ptr, or NULL when ptr is not tracked. */
static boot_record_t *boot_take_record(void *ptr) {
  boot_record_t **link = &boot_records;
  while (*link != NULL) {
    if ((*link)->ptr == ptr) {
      boot_record_t *found = *link;
      *link = found->next;
      return found;
    }
    link = &(*link)->next;
  }
  return NULL;
}

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

void *boot_realloc(void *ptr, size_t size) {
  boot_reallocs++;
  boot_last_realloc = size;

  /* realloc(NULL, n) is defined to behave as malloc(n). */
  if (ptr == NULL) {
    return boot_malloc(size);
  }

  /* realloc(p, 0) is implementation defined and became undefined in C23.
   * Treat it as a free so the bookkeeping cannot drift. */
  if (size == 0) {
    boot_free(ptr);
    return NULL;
  }

  /* Unlink before reallocating. After realloc succeeds the old pointer value
   * is indeterminate, so it must not be compared against the records. */
  boot_record_t *rec = boot_take_record(ptr);

  void *newptr = realloc(ptr, size);
  if (newptr == NULL) {
    /* The original block is still valid, so restore its record unchanged. */
    if (rec != NULL) {
      rec->next = boot_records;
      boot_records = rec;
    }
    return NULL;
  }

  if (rec == NULL) {
    /* Untracked input, so the result stays untracked too. */
    return newptr;
  }

  boot_bytes -= rec->size;
  boot_bytes += size;
  rec->ptr = newptr;
  rec->size = size;
  rec->next = boot_records;
  boot_records = rec;
  return newptr;
}

void boot_free(void *ptr) {
  /* free(NULL) is a no-op that was never counted, so it must not decrement. */
  if (ptr == NULL) {
    return;
  }

  boot_record_t *dead = boot_take_record(ptr);
  if (dead != NULL) {
    boot_outstanding--;
    boot_bytes -= dead->size;
    free(dead);
  }

  /* An untracked pointer, from a file that does not include this header, is
   * still released but leaves the counters alone. */
  free(ptr);
}

size_t boot_alloc_count(void) { return boot_outstanding; }

size_t boot_alloc_size(void) { return boot_bytes; }

size_t boot_realloc_count(void) { return boot_reallocs; }

size_t boot_last_realloc_size(void) { return boot_last_realloc; }

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
  boot_reallocs = 0;
  boot_last_realloc = 0;
}
