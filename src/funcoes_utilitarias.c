#include "funcoes.h"
#include <stdio.h>
#include <string.h>

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

void getStatus(int status, char *str) {
  switch (status) {
  case STATUS_OPEN:
    strcpy(str, "Aberto");
    break;
  case STATUS_WAITING_USER:
    strcpy(str, "Esp. Tecnico");
    break;
  case STATUS_IN_PROGRESS:
    strcpy(str, "Em Atend.");
    break;
  case STATUS_RESOLVED:
    strcpy(str, "Resolvido");
    break;
  case STATUS_CLOSED:
    strcpy(str, "Fechado");
    break;
  default:
    strcpy(str, "???");
    break;
  }
}

void getPriority(int prioridade, char *texto) {
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

int confirmDelete() {
  int confirmDelete;
  do {
    puts("Confirmar remoção?");
    puts("0 - Continuar");
    puts("1 - Cancelar");

    if (scanf("%d", &confirmDelete) != 1 ||
        (confirmDelete != 0 && confirmDelete != 1)) {
      clearBuffer();
      puts("Insira uma opção válida");
    } else {
      break;
    }

  } while (1);

  return confirmDelete;
}

void getType(int typeId, char *str, ELEM_TICKET_TYPE *headTicketsTypes) {
  ELEM_TICKET_TYPE *temp = headTicketsTypes;

  while (temp != NULL) {
    if (temp->data.id == typeId) {
      strcpy(str, temp->data.name);
      return;
    }
    temp = temp->next;
  }
  strcpy(str, "???");
}

DateTime addTimeToDateTime(DateTime dt, int hours, int minutes) {
  DateTime resultado = dt;

  // EX -> Hora atual(resultado) = 12:30
  // Exemplo base -> se chegar 100 minutos via parametro somo aos minutos do
  // resultado 12:130
  resultado.min += minutes;
  resultado.hour += hours;

  // Aqui enquanto for maior ou igual a 60 subtraio 60 e aumenta 1 hora
  while (resultado.min >= 60) {
    resultado.min -= 60;
    resultado.hour++;
  }

  // O mesmo para as horas, se tiver mais de 24h passo para o dia seguite
  while (resultado.hour >= 24) {
    resultado.hour -= 24;
    resultado.day++;
  }

  // Já tá feita a logica para os meses 31/30 direitinho
  while (resultado.day > diasNoMes(resultado.month, resultado.year)) {
    resultado.day -= diasNoMes(resultado.month, resultado.year);
    resultado.month++;
  }

  // Ajustar meses
  while (resultado.month > 12) {
    resultado.month -= 12;
    resultado.year++;
  }

  return resultado;
}

int diasNoMes(int month, int year) {
  switch (month) {
  case 1:
    return 31; // janeiro
  case 2:      // fevereiro - verificar ano bissexto
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
      return 29;
    else
      return 28;
  case 3:
    return 31; // março
  case 4:
    return 30; // abril
  case 5:
    return 31; // maio
  case 6:
    return 30; // junho
  case 7:
    return 31; // julho
  case 8:
    return 31; // agosto
  case 9:
    return 30; // setembro
  case 10:
    return 31; // outubro
  case 11:
    return 30; // novembro
  case 12:
    return 31; // dezembro
  default:
    return 30;
  }
}