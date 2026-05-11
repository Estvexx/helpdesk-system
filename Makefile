CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = src/main.c src/funcoes.c
OUT = programa

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)