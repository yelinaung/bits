#!/usr/bin/env bash
# Build and run every munit test suite in the repo.
#
# Each entry below is "<test main>|<space-separated sources>". The build goes
# through the same `make test` target used locally, so CI and your terminal
# compile with identical flags.
#
#   ./run-tests.sh            run everything
#   ./run-tests.sh snekstack  run only tests whose name matches "snekstack"
#
# Exit status is 0 only when every test that is expected to pass does pass.

set -uo pipefail
cd "$(dirname "$0")"

MS_SRCS="ms/sneknew.c ms/snekobject.c ms/stack.c ms/vm.c"

TESTS=(
  # -- basics ------------------------------------------------------------
  "enum_test.c|"
  "enum_http_switch_test.c|enum_http.c"
  "enum_ex1_exercise.c|enum_ex1.c"
  "union_main_test.c|union_main_exercise.c"
  "union_tcp_test.c|"

  # -- heap and pointers -------------------------------------------------
  "heap_main_test.c|heap_main_exercise.c"
  "malloc_exercise_test.c|malloc_exercise.c"
  "pointer_to_pointer_test.c|pointer_to_pointer_exercise.c"
  "aop_main.c|aop_exercise.c"
  "swap_int_main.c|swap_int_exercise.c"
  "swap_string_main.c|swap_string_exercise.c"
  "swap_generic_main.c|swap_generic_exercise.c"
  "void_pointer_main.c|void_pointer_exercise.c"

  # -- snek objects ------------------------------------------------------
  "snek_obj_main.c|"
  "snek_obj_integer.c|snek_obj.c"
  "snek_obj_float.c|snek_obj.c"
  "snek_obj_str.c|snek_obj.c"
  "snek_obj_vec3.c|snek_obj.c"
  "snek_obj_array.c|snek_obj.c"
  "snek_obj_array_get.c|snek_obj.c"
  "snek_obj_array_set.c|snek_obj.c"
  "snek_obj_array_length.c|snek_obj.c"
  "snek_obj_add.c|snek_obj.c"

  # -- reference counting ------------------------------------------------
  "snek_obj_ref_count.c|snek_obj.c"
  "snek_obj_ref_count_inc.c|snek_obj.c"
  "snek_obj_ref_count_decre.c|snek_obj.c"
  "snek_obj_ref_count_vec.c|snek_obj.c"
  "snek_obj_ref_count_array.c|snek_obj.c"
  "snek_obj_gc_cycles.c|snek_obj.c"

  # -- snek stack --------------------------------------------------------
  "snekstack_main.c|snekstack.c snek_obj.c"
  "snekstack_push.c|snekstack.c snek_obj.c"
  "snekstack_pop.c|snekstack.c snek_obj.c"
  "snekstack_free.c|snekstack.c snek_obj.c"
  "snekstack_hetrogenous_stack.c|snekstack_hetro.c snekstack.c snek_obj.c"
  "snekstack_multiple_types_main.c|snekstack_multiple_types.c snekstack.c snek_obj.c"

  # -- mark and sweep ----------------------------------------------------
  "ms/main_05_free.c|$MS_SRCS"
  "ms/main_06_frame_ref.c|$MS_SRCS"
  "ms/main_07_mark_and_sweep_.c|$MS_SRCS"
  "ms/main_08_mark.c|$MS_SRCS"
  "ms/main_09_trace.c|$MS_SRCS"
  "ms/main_10_sweep.c|$MS_SRCS"

  # Others
  "stack_ex_tests.c|stack_ex.c"
)

# Tests known to fail. A failure here is reported but does not fail the run.
# A test in this list that starts passing is reported too, so the list stays
# honest. Remove the entry once you fix it.
KNOWN_FAILING=(
  "snek_obj_str.c"        # bootlib byte count disagrees with the assertion
  "snek_obj_gc_cycles.c"  # refcount cannot break cycles; superseded by ms/
)

# Superseded by the ms/ folder and no longer buildable:
#   snek_ms_vm_main.c, snek_ms_vm_stackframes.c, snek_obj_new_with_vm.c
#   (snek_obj.h defines snek_vector_t twice)

filter="${1:-}"
pass=0; fail=0; xfail=0; xpass=0
failed_names=()

is_known_failing() {
  local name="$1" k
  for k in "${KNOWN_FAILING[@]}"; do
    [[ "$name" == "$k" ]] && return 0
  done
  return 1
}

for entry in "${TESTS[@]}"; do
  main="${entry%%|*}"
  srcs="${entry#*|}"

  if [[ -n "$filter" && "$main" != *"$filter"* ]]; then
    continue
  fi

  log=$(mktemp)
  if make test "$main" SRCS="$srcs vendor/bootlib.c" ARGS=--no-fork >"$log" 2>&1; then
    if is_known_failing "$main"; then
      printf 'XPASS %s  (listed as known-failing; remove it)\n' "$main"
      xpass=$((xpass + 1))
    else
      printf 'ok    %s\n' "$main"
      pass=$((pass + 1))
    fi
  else
    if is_known_failing "$main"; then
      printf 'xfail %s  (known)\n' "$main"
      xfail=$((xfail + 1))
    else
      printf 'FAIL  %s\n' "$main"
      sed -n '/Running test suite/,$p' "$log" | head -40 | sed 's/^/      /'
      fail=$((fail + 1))
      failed_names+=("$main")
    fi
  fi
  rm -f "$log"
done

echo
printf '%d passed, %d failed, %d known-failing' "$pass" "$fail" "$xfail"
[[ $xpass -gt 0 ]] && printf ', %d unexpectedly passing' "$xpass"
echo

if [[ $fail -gt 0 ]]; then
  printf '\nFailed:\n'
  printf '  %s\n' "${failed_names[@]}"
  exit 1
fi
exit 0
