CC = gcc
CFLAGS = -Wall -Wextra

programme: main.c
	$(CC) $(CFLAGS) -o programme main.c

run: programme
	./programme

clean:
	rm -f programme