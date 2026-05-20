CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o utils.o plateau.o jeu.o
EXEC = memo_rpg

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
