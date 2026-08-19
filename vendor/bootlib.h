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
 */

#ifndef BOOTLIB_H
#define BOOTLIB_H

#include <stdbool.h>
#include <stdlib.h>

void *boot_malloc(size_t size);
void boot_free(void *ptr);

/* Number of allocations made through boot_malloc that are not yet freed. */
size_t boot_alloc_count(void);

/* True when every tracked allocation has been released. */
bool boot_all_freed(void);

/* Reset the counter. Useful between test cases. */
void boot_reset(void);

#ifndef BOOTLIB_INTERNAL
#define malloc(size) boot_malloc(size)
#define free(ptr) boot_free(ptr)
#endif

#endif /* BOOTLIB_H */
