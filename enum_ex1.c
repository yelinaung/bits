#include "enum_ex1.h"
#include <stddef.h>

void reset_counts(status_counts_t *counts) {
  if (counts == NULL) {
    return;
  }
  counts->pending = 0;
  counts->done = 0;
  counts->failed = 0;
  counts->unknown = 0;
}

void add_status(status_counts_t *counts, status_t status) {
  if (counts == NULL) {
    return;
  }
  switch (status) {
  case STATUS_PENDING:
    counts->pending += 1;
    break;
  case STATUS_DONE:
    counts->done += 1;
    break;
  case STATUS_FAILED:
    counts->failed += 1;
    break;
  default:
    counts->unknown += 1;
    break;
  }
}

void count_statuses(const status_t *statuses, int length,
                    status_counts_t *counts) {
  if (counts == NULL) {
    return;
  }
  if (statuses == NULL || length <= 0) {
    reset_counts(counts);
    return;
  }

  reset_counts(counts);
  for (int i = 0; i < length; i++) {
    add_status(counts, statuses[i]);
  }
}
