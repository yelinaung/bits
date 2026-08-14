CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address -Wconversion

MUNIT_DIR = vendor/munit

# Usage: make run p.c
#        make test test_smart_s.c SRCS="smart_s.c"
#        make test test_smart_s.c SRCS="smart_s.c" ARGS=--no-fork
ifneq (,$(filter run test,$(firstword $(MAKECMDGOALS))))
  FILE := $(word 2,$(MAKECMDGOALS))
  .PHONY: $(FILE)
  $(FILE):
	@:
endif

run:
	$(CC) $(CFLAGS) -o /tmp/$(basename $(FILE)) $(FILE)
	/tmp/$(basename $(FILE))

test:
	$(CC) $(CFLAGS) -I$(MUNIT_DIR) -o /tmp/$(basename $(FILE)) \
		$(FILE) $(SRCS) $(MUNIT_DIR)/munit.c
	/tmp/$(basename $(FILE)) $(ARGS)

clean:
	rm -f /tmp/a1 /tmp/arr /tmp/p /tmp/play /tmp/playground

.PHONY: run test clean
