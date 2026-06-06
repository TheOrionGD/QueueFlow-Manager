CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c seat.c queue.c file_io.c
OBJS = $(SRCS:.c=.o)
EXEC = waiting_list

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
