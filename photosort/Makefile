SRCS = $(wildcard *.c)
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -g -fsanitize=address -fno-omit-frame-pointer -O0
LIBS = -lm -lraylib
TARGETS = $(SRCS:.c=.macho) $(SRCS:.c=.exe)

all: $(TARGETS)

%.macho: %.c
	# use clang to compile for macos
	clang -o $@ $(CFLAGS) $(LIBS) $<

%.exe: %.c
	# use mingw to compile for windows
	x86_64-w64-mingw32-gcc -o $@ $(CFLAGS) $(LIBS) $<

clean:
	rm -f $(TARGETS)
	rm -f *.o

.PHONY: clean
