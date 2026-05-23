CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: memo_rpg

memo_rpg: main.o utils.o plateau.o jeu.o stats.o
	$(CC) $(CFLAGS) -o memo_rpg main.o utils.o plateau.o jeu.o stats.o

main.o: main.c structures.h utils.h plateau.h jeu.h stats.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c utils.h structures.h
	$(CC) $(CFLAGS) -c utils.c

plateau.o: plateau.c plateau.h structures.h utils.h
	$(CC) $(CFLAGS) -c plateau.c

jeu.o: jeu.c jeu.h structures.h utils.h
	$(CC) $(CFLAGS) -c jeu.c

stats.o: stats.c stats.h structures.h utils.h
	$(CC) $(CFLAGS) -c stats.c

clean:
	rm -f *.o memo_rpg
