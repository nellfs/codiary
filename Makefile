CC = gcc
CFLAGS = -Wall -Wextra -std=c17

SRC = src/main.c src/storage.c src/datetime.c src/cli.c
OBJ = $(SRC:.c=.o)

TARGET = dia

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rule for .o files
%.o: %.c src/storage.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
