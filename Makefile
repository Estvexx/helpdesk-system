CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = src/main.c src/funcoes_users.c src/funcoes_ticket.c
OUT = exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
	.\$(OUT).exe

clean:
	rm -f $(OUT)

.PHONY: all clean run