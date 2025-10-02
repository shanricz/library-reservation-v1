CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I src/headers -I src

SRC = \
	src/MainProgram.c \
	src/db.c \
	src/reservation.c \
	src/utilities.c \
	src/sqlite3.c

OBJ = $(SRC:.c=.o)
BIN = LibraryReservation.exe

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@

clean:
	rm -f $(OBJ) $(BIN)