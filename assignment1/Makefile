# Compiler is GCC
CC=gcc
# CFLAGS is the options to pass to the compiler.
# -c means to create an object file.
CFLAGS=-ansi -pedantic -Wall -g -c

all: solitaire

solitaire: solitaire.o board.o game.o utility.o
# $^ means "all dependencies of this target"
# $@ means "the name of this target"
	$(CC) $^ -o $@

solitaire.o: solitaire.c solitaire.h
	$(CC) $(CFLAGS) solitaire.c

board.o: board.c board.h
	$(CC) $(CFLAGS) board.c

game.o: game.c game.h
	$(CC) $(CFLAGS) game.c

utility.o: utility.c utility.h
	$(CC) $(CFLAGS) utility.c

clean:
	rm -rf *.o solitaire *~
