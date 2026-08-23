/* Boot.dev munit compatibility shim.
 *
 * The Boot.dev C courses ship a modified munit.h with convenience macros that
 * upstream nemequ/munit does not have. That fork is delivered inside the lesson
 * scaffolding and is not publicly downloadable, so this header reimplements the
 * macros on top of stock munit 0.4.1.
 *
 * This is a reconstruction from call sites, not a copy of Boot.dev's header.
 * See the notes on RUN/SUBMIT and on assert message formatting below.
 */

#ifndef MUNIT_BOOTDEV_H
#define MUNIT_BOOTDEV_H

#include "munit.h"

/* Boot.dev tags each test RUN or SUBMIT: RUN executes when you hit Run, SUBMIT
 * only on submission. Locally there is no such distinction, so both run. The
 * mode argument is accepted and ignored. */
#define RUN munit_mode_run
#define SUBMIT munit_mode_submit
enum { munit_mode_run = 0, munit_mode_submit = 1 };

/* munit_case(MODE, name, { body }) -> a MunitResult function.
 * The trailing semicolon at the call site lands on the empty declaration.
 *
 * The body is taken as __VA_ARGS__, not a single parameter. The preprocessor
 * only treats parentheses as grouping, never braces, so a body that contains
 * a brace initializer such as
 *     token_t tok = {"hello", 1, 1};
 * splits into extra macro arguments and fails with "passed 5 arguments, but
 * takes just 3". Variadic collection puts the commas back together. */
#define munit_case(mode, name, ...)                                            \
  static MunitResult name(const MunitParameter params[], void *user_data) {    \
    (void)params;                                                              \
    (void)user_data;                                                           \
    (void)(mode);                                                              \
    __VA_ARGS__ return MUNIT_OK;                                               \
  }                                                                            \
  struct munit_case_semicolon_##name

#define munit_test(name_, fn_)                                                 \
  {(char *)(name_), fn_, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}

#define munit_null_test {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}

#define munit_suite(name_, tests_)                                             \
  {(char *)(name_), tests_, NULL, 1, MUNIT_SUITE_OPTION_NONE}

/* Boot.dev's assert_* take a trailing description; upstream's do not.
 * Upstream already defines the 3-argument forms, so undefine first.
 * Operands are copied into temporaries so arguments with side effects are
 * evaluated exactly once. */
#undef assert_int
#undef assert_uint
#undef assert_long
#undef assert_ulong
#undef assert_char
#undef assert_ptr
#undef assert_string_equal

#define munit_bootdev_assert_(type, fmt, a, op, b, msg)                        \
  do {                                                                         \
    type munit_lhs_ = (type)(a);                                               \
    type munit_rhs_ = (type)(b);                                               \
    if (!(munit_lhs_ op munit_rhs_))                                           \
      munit_errorf("%s (" fmt " " #op " " fmt ")", (msg), munit_lhs_,          \
                   munit_rhs_);                                                \
  } while (0)

/* Both spellings exist at call sites for the scalar compares too. Define the
 * munit_-prefixed form and alias the bare one, so the two cannot drift.
 * Note munit_assert_ptr is deliberately left alone: upstream builds
 * munit_assert_null/not_null/ptr_equal on top of it, and widening it to four
 * arguments would break those. munit_assert_not_null is written out separately
 * below instead. */
#undef munit_assert_int
#undef munit_assert_not_null
#undef munit_assert_null

#define munit_assert_int(a, op, b, msg)                                        \
  munit_bootdev_assert_(int, "%d", a, op, b, msg)

#define munit_assert_not_null(ptr, msg)                                        \
  do {                                                                         \
    const void *munit_ptr_ = (ptr);                                            \
    if (munit_ptr_ == NULL)                                                    \
      munit_errorf("%s (pointer is NULL)", (msg));                             \
  } while (0)

#define munit_assert_null(ptr, msg)                                            \
  do {                                                                         \
    const void *munit_ptr_ = (ptr);                                            \
    if (munit_ptr_ != NULL)                                                    \
      munit_errorf("%s (pointer is %p, expected NULL)", (msg), munit_ptr_);    \
  } while (0)

#define assert_int(a, op, b, msg) munit_assert_int(a, op, b, msg)

/* Boolean checks take no description at call sites, matching upstream arity. */
#define assert_true(expr) munit_assert_true(expr)
#define assert_false(expr) munit_assert_false(expr)

/* Call sites use assert_ptr_not_null and assert_not_null interchangeably for
 * the same check. Upstream's munit_assert_ptr_not_null stays 1-argument. */
#define assert_not_null(ptr, msg) munit_assert_not_null(ptr, msg)
#define assert_ptr_not_null(ptr, msg) munit_assert_not_null(ptr, msg)
#define assert_null(ptr, msg) munit_assert_null(ptr, msg)
#define assert_ptr_null(ptr, msg) munit_assert_null(ptr, msg)

#undef munit_assert_ptr_not_equal
#define munit_assert_ptr_not_equal(a, b, msg)                                  \
  munit_bootdev_assert_(const void *, "%p", a, !=, b, msg)
#define assert_ptr_not_equal(a, b, msg) munit_assert_ptr_not_equal(a, b, msg)

/* The *_equal spellings fix the operator and take only the two operands. */
#undef munit_assert_ptr_equal
#define munit_assert_ptr_equal(a, b, msg)                                      \
  munit_bootdev_assert_(const void *, "%p", a, ==, b, msg)
#define assert_ptr_equal(a, b, msg) munit_assert_ptr_equal(a, b, msg)
#define assert_int_equal(a, b, msg) munit_assert_int(a, ==, b, msg)
#define assert_uint(a, op, b, msg)                                             \
  munit_bootdev_assert_(unsigned int, "%u", a, op, b, msg)
#define assert_long(a, op, b, msg)                                             \
  munit_bootdev_assert_(long, "%ld", a, op, b, msg)
#define assert_ulong(a, op, b, msg)                                            \
  munit_bootdev_assert_(unsigned long, "%lu", a, op, b, msg)
#define assert_char(a, op, b, msg)                                             \
  munit_bootdev_assert_(char, "%c", a, op, b, msg)
#define assert_ptr(a, op, b, msg)                                              \
  munit_bootdev_assert_(const void *, "%p", a, op, b, msg)

/* The munit_assert_* sized forms take a trailing description too. Same
 * treatment: undefine upstream's 3-argument versions and rebuild them.
 * Unsigned sized types print as hex, since call sites use them for byte and
 * bit layouts where "0x34 != 0x1234" reads better than "52 != 4660". */
#undef munit_assert_size
#undef munit_assert_float
#undef munit_assert_double
#undef munit_assert_int8
#undef munit_assert_uint8
#undef munit_assert_int16
#undef munit_assert_uint16
#undef munit_assert_int32
#undef munit_assert_uint32
#undef munit_assert_int64
#undef munit_assert_uint64
#undef munit_assert_string_equal

#define munit_assert_size(a, op, b, msg)                                       \
  munit_bootdev_assert_(size_t, "%" MUNIT_SIZE_MODIFIER "u", a, op, b, msg)
#define assert_size(a, op, b, msg) munit_assert_size(a, op, b, msg)

/* float and double both widen to double when passed to munit_errorf, so both
 * print with "%f". Exact comparison, matching upstream: use these only for
 * values that are computed exactly, such as a zeroed field. */
#define munit_assert_float(a, op, b, msg)                                      \
  munit_bootdev_assert_(float, "%f", a, op, b, msg)
#define munit_assert_double(a, op, b, msg)                                     \
  munit_bootdev_assert_(double, "%f", a, op, b, msg)
#define assert_float(a, op, b, msg) munit_assert_float(a, op, b, msg)
#define assert_double(a, op, b, msg) munit_assert_double(a, op, b, msg)

#define munit_assert_int8(a, op, b, msg)                                       \
  munit_bootdev_assert_(munit_int8_t, "%" PRIi8, a, op, b, msg)
#define munit_assert_uint8(a, op, b, msg)                                      \
  munit_bootdev_assert_(munit_uint8_t, "0x%02" PRIx8, a, op, b, msg)
#define munit_assert_int16(a, op, b, msg)                                      \
  munit_bootdev_assert_(munit_int16_t, "%" PRIi16, a, op, b, msg)
#define munit_assert_uint16(a, op, b, msg)                                     \
  munit_bootdev_assert_(munit_uint16_t, "0x%04" PRIx16, a, op, b, msg)
#define munit_assert_int32(a, op, b, msg)                                      \
  munit_bootdev_assert_(munit_int32_t, "%" PRIi32, a, op, b, msg)
#define munit_assert_uint32(a, op, b, msg)                                     \
  munit_bootdev_assert_(munit_uint32_t, "0x%08" PRIx32, a, op, b, msg)
#define munit_assert_int64(a, op, b, msg)                                      \
  munit_bootdev_assert_(munit_int64_t, "%" PRIi64, a, op, b, msg)
#define munit_assert_uint64(a, op, b, msg)                                     \
  munit_bootdev_assert_(munit_uint64_t, "0x%016" PRIx64, a, op, b, msg)

/* Both spellings of the string compare take a trailing description. Call sites
 * use munit_assert_string_equal and the bare assert_string_equal alias
 * interchangeably, so define one and forward the other. */
#define munit_assert_string_equal(a, b, msg)                                   \
  do {                                                                         \
    const char *munit_lhs_ = (a);                                              \
    const char *munit_rhs_ = (b);                                              \
    if (strcmp(munit_lhs_, munit_rhs_) != 0)                                   \
      munit_errorf("%s (\"%s\" != \"%s\")", (msg), munit_lhs_, munit_rhs_);    \
  } while (0)

#define assert_string_equal(a, b, msg) munit_assert_string_equal(a, b, msg)

#endif /* MUNIT_BOOTDEV_H */
