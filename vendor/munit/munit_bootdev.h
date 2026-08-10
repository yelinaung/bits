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
 * The trailing semicolon at the call site lands on the empty declaration. */
#define munit_case(mode, name, body)                                           \
  static MunitResult name(const MunitParameter params[], void *user_data) {    \
    (void)params;                                                              \
    (void)user_data;                                                           \
    (void)(mode);                                                              \
    body return MUNIT_OK;                                                      \
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

#define assert_int(a, op, b, msg)                                              \
  munit_bootdev_assert_(int, "%d", a, op, b, msg)
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

#define assert_string_equal(a, b, msg)                                         \
  do {                                                                         \
    const char *munit_lhs_ = (a);                                              \
    const char *munit_rhs_ = (b);                                              \
    if (strcmp(munit_lhs_, munit_rhs_) != 0)                                   \
      munit_errorf("%s (\"%s\" != \"%s\")", (msg), munit_lhs_, munit_rhs_);    \
  } while (0)

#endif /* MUNIT_BOOTDEV_H */
