CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address

# Usage: make run p.c
ifeq (run,$(firstword $(MAKECMDGOALS)))
  FILE := $(word 2,$(MAKECMDGOALS))
  .PHONY: $(FILE)
  $(FILE):
	@:
endif

run:
	$(CC) $(CFLAGS) -o /tmp/$(basename $(FILE)) $(FILE)
	/tmp/$(basename $(FILE))

clean:
	rm -f /tmp/a1 /tmp/arr /tmp/p /tmp/play /tmp/playground
