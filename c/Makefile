SRCS = tree.c list.c
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -g -fsanitize=address -fno-omit-frame-pointer -O0
TARGETS = tree.x list.x queue.x ray.x

# Ray tracer specific flags
RAY_LIBS = -lm -lraylib

all: $(TARGETS) ray.x
	for target in $(TARGETS); do \
		./$$target; \
		done

# Standard C files
%.x: %.c
	clang -o $@ $(CFLAGS) $(LIBS) $<

# Ray tracer specific build
ray.x: ray.c
	clang -o $@ $(CFLAGS) $(RAY_LIBS) $<

clean:
	rm -f $(TARGETS) ray.x
	rm -f *.o


.PHONY: all clean 