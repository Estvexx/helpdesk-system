#include "funcoes.h"
#include <stdio.h>

void clearBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void waitForKey() {
  puts("\nCarregue numa tecla para continuar ...");
  getchar();
}

// Retorna 1 se d1 > d2, 0 se igual, -1 se d1 < d2
int compareDates(DateTime d1, DateTime d2) {
  if (d1.year > d2.year)
    return 1;
  if (d1.year < d2.year)
    return -1;
  if (d1.month > d2.month)
    return 1;
  if (d1.month < d2.month)
    return -1;
  if (d1.day > d2.day)
    return 1;
  if (d1.day < d2.day)
    return -1;
  if (d1.hour > d2.hour)
    return 1;
  if (d1.hour < d2.hour)
    return -1;
  if (d1.min > d2.min)
    return 1;
  if (d1.min < d2.min)
    return -1;
  return 0;
}

void obterTipo(int tipo, char *texto) {
  switch (tipo) {
  case TYPE_HARDWARE:
    strcpy(texto, "Hardware");
    break;
  case TYPE_SOFTWARE:
    strcpy(texto, "Software");
    break;
  case TYPE_NETWORK:
    strcpy(texto, "Rede");
    break;
  case TYPE_ACCESS:
    strcpy(texto, "Acesso");
    break;
  case TYPE_OTHER:
    strcpy(texto, "Outro");
    break;
  default:
    strcpy(texto, "???");
    break;
  }
}

void obterEstado(int status, char *texto) {
  switch (status) {
  case STATUS_OPEN:
    strcpy(texto, "Aberto");
    break;
  case STATUS_IN_PROGRESS:
    strcpy(texto, "Em Atend.");
    break;
  case STATUS_WAITING_USER:
    strcpy(texto, "Esp. User");
    break;
  case STATUS_RESOLVED:
    strcpy(texto, "Resolvido");
    break;
  case STATUS_CLOSED:
    strcpy(texto, "Fechado");
    break;
  default:
    strcpy(texto, "???");
    break;
  }
}

void obterPrioridade(int prioridade, char *texto) {
  switch (prioridade) {
  case 1:
    strcpy(texto, "Baixa");
    break;
  case 2:
    strcpy(texto, "Media");
    break;
  case 3:
    strcpy(texto, "Alta");
    break;
  case 4:
    strcpy(texto, "Critica");
    break;
  default:
    strcpy(texto, "???");
    break;
  }
}

void tableHeaders() {
  printf("\n%-5s | %-9s | %-12s | %-10s | %-15s | %-10s |%-16s\n", "ID", "Tipo",
         "Estado", "Prioridade", "Utilizador", "Tecnico", "Data Abertura");
  printf("------+-----------+--------------+------------+-----------------+----"
         "--------+-----------------\n");
}