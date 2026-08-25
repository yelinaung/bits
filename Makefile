CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address -Wconversion -g

BIN = /tmp/$(subst /,_,$(basename $(FILE)))

VENDOR_DIR = vendor
MUNIT_DIR = vendor/munit
INCLUDES = -I$(VENDOR_DIR) -I$(MUNIT_DIR)

# Usage: make run p.c
#        make run free_exercise_main.c SRCS="free_exercise.c"
#        make test test_smart_s.c SRCS="smart_s.c"
#        make test test_smart_s.c SRCS="smart_s.c" ARGS=--no-fork
ifneq (,$(filter run test,$(firstword $(MAKECMDGOALS))))
  FILE := $(word 2,$(MAKECMDGOALS))
  .PHONY: $(FILE)
  $(FILE):
	@:
endif

run:
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN) $(FILE) $(SRCS)
	$(BIN)

test:
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BIN) \
		$(FILE) $(SRCS) $(MUNIT_DIR)/munit.c
	$(BIN) $(ARGS)

clean:
	rm -f /tmp/a1 /tmp/arr /tmp/p /tmp/play /tmp/playground

.PHONY: run test clean
