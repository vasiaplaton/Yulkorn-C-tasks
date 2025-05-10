CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -Iinclude

SRC     = src/main.c src/cli.c src/udt_list.c src/io_file.c src/sort_variant.c
BINDIR  = bin
OBJS    = $(patsubst src/%.c,$(BINDIR)/%.o,$(SRC))
EXEC    = lab3
BINARY  = $(BINDIR)/$(EXEC)

.PHONY: all clean run

all: $(BINARY)

$(BINARY): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/%.o: src/%.c
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(BINARY)
	-rmdir --ignore-fail-on-non-empty $(BINDIR)

run: $(BINARY)
	@./$(BINARY)
