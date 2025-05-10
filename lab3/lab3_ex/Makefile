CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRC = src/cli.c src/deque_copy.c src/deque.c src/io_file.c src/main.c src/sort.c
OBJS = $(SRC:.c=.o)
EXEC = lab3

.PHONY: all clean run  

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	@./$(EXEC)