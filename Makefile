CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra
OBJS = bitops.o game.o main.o

all: checkers

bitops.o: bitops.c bitops.h
	$(CC) $(CFLAGS) -c bitops.c -o bitops.o

game.o: game.c game.h
	$(CC) $(CFLAGS) -c game.c -o game.o

main.o: main.c bitops.h game.h
	$(CC) $(CFLAGS) -c main.c -o main.o

checkers: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o checkers

clean:
	rm -f *.o checkers
