/* Boot.dev bootlib compatibility shim.
 *
 * The Boot.dev C courses ship a bootlib that tracks heap allocations so tests
 * can assert a function released everything it took. That library is delivered
 * inside the lesson scaffolding and is not publicly downloadable, so this pair
 * of files reimplements the part the exercises use.
 *
 * This is a reconstruction from call sites, not a copy of Boot.dev's library.
 *
 * Include this header in any translation unit whose allocations should be
 * counted. malloc and free are redirected to counting wrappers by macro, so
 * the calling code needs no other change. Files that must reach the real
 * allocator define BOOTLIB_INTERNAL before including.
 *
 * Not thread safe. The counters are plain globals, which is sufficient for
 * single threaded test binaries.
 */

#ifndef BOOTLIB_H
#define BOOTLIB_H

#include <stdbool.h>
#include <stdlib.h>

void *boot_malloc(size_t size);
void *boot_calloc(size_t nmemb, size_t size);
void *boot_realloc(void *ptr, size_t size);
void boot_free(void *ptr);

/* Number of tracked allocations that are not yet freed. */
size_t boot_alloc_count(void);

/* Total bytes of tracked allocations that are not yet freed. */
size_t boot_alloc_size(void);

/* Number of boot_realloc calls so far, and the size requested by the most
 * recent one. Both are cumulative and are only cleared by boot_reset. */
size_t boot_realloc_count(void);
size_t boot_last_realloc_size(void);

/* True when ptr was allocated through this library and has since been freed.
 * False while ptr is still live, and false for pointers never tracked here.
 * If an address is recycled by a later allocation, live wins over freed. */
bool boot_is_freed(const void *ptr);

/* True when every tracked allocation has been released. */
bool boot_all_freed(void);

/* Drop all bookkeeping and zero the counters. Useful between test cases. */
void boot_reset(void);

#ifndef BOOTLIB_INTERNAL
#define malloc(size) boot_malloc(size)
#define calloc(nmemb, size) boot_calloc(nmemb, size)
#define realloc(ptr, size) boot_realloc(ptr, size)
#define free(ptr) boot_free(ptr)
#endif

#endif /* BOOTLIB_H */
