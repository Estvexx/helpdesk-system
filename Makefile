CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = src/main.c src/funcoes.c
OUT = exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)

.PHONY: all clean run