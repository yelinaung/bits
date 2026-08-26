#pragma once

typedef enum Status {
  STATUS_PENDING,
  STATUS_DONE,
  STATUS_FAILED,
} status_t;

typedef struct StatusCounts {
  status_t pending;
  status_t done;
  status_t failed;
  status_t unknown;
} status_counts_t;

void reset_counts(status_counts_t *counts);
void add_status(status_counts_t *counts, status_t status);
void count_statuses(const status_t *statuses, int length,
                    status_counts_t *counts);
