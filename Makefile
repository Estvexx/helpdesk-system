CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = src/main.c src/funcoes_users.c src/funcoes_ticket.c src/funcoes_utilitarias.c src/funcoes_historico.c src/funcoes_ticketTypes.c src/funcoes_exit.c src/persistencia.c src/ui/ui.c src/input/input.c src/logs/log.c
OUT = exe
DATAOUT = data/*.dat

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
	.\$(OUT).exe

cleardata:
	rm -f $(DATAOUT)

clean:
	rm -f $(OUT)

.PHONY: all clean run
