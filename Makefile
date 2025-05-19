CC = gcc
CFLAGS = -Wall -Wextra -I. -Icli-lib/include
LDFLAGS = -L./cli-lib/lib -lcli-lib

SRC = codigo/main.c codigo/game.c
OBJ = $(SRC:.c=.o)
EXEC = pong

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean