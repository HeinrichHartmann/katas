SRCS = tree.c
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -g -fsanitize=address -fno-omit-frame-pointer
OBJS = $(SRCS:.c=.x)

%.x: %.c
	clang -o $@ $(CFLAGS) $(LIBS) $<

build: $(OBJS)
	# pass

run: tree.x
	./tree.x

debug: a.out
	lldb ./a.out
