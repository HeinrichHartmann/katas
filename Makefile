CFLAGS = -g -Wall -Wextra -Werror -std=c99 -pedantic -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

TARGETS = main.x

%.x: %.c
	clang $(CFLAGS) -o $@ $<

all: $(TARGETS)
