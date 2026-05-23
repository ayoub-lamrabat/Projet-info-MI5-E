CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o utils.o plateau.o jeu.o stats.o
EXEC = memo_rpg

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

main.o: main.c structures.h utils.h plateau.h jeu.h stats.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c utils.h structures.h
	$(CC) $(CFLAGS) -c utils.c

plateau.o: plateau.c plateau.h utils.h structures.h
	$(CC) $(CFLAGS) -c plateau.c

jeu.o: jeu.c jeu.h utils.h structures.h
	$(CC) $(CFLAGS) -c jeu.c

stats.o: stats.c stats.h structures.h
	$(CC) $(CFLAGS) -c stats.c

clean:
	rm -f $(OBJ) $(EXEC)
