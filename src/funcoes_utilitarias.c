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
    strcpy(str, "Espera de Tecnico");
    break;
  case STATUS_IN_PROGRESS:
    strcpy(str, "Em Atendimento");
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

void getPriority(int priority, char *str) {
  switch (priority) {
  case 1:
    strcpy(str, "Baixa");
    break;
  case 2:
    strcpy(str, "Media");
    break;
  case 3:
    strcpy(str, "Alta");
    break;
  case 4:
    strcpy(str, "Critica");
    break;
  default:
    strcpy(str, "???");
    break;
  }
}

int getSLA(int priority) {
  switch (priority) {
  case 1:
    return 2880; // Baixa
  case 2:
    return 1440; // Media
  case 3:
    return 240; // Alta
  case 4:
    return 120; // Critica
  default:
    return -1; // Erro
  }
}

void tableHeaders() {
  printf("\n%-5s | %-9s | %-12s | %-10s | %-15s | %-10s | %-16s | %-5s\n", "ID",
         "Tipo", "Estado", "Prioridade", "Utilizador", "Tecnico",
         "Data Abertura", "SLA");
  printf("------+-----------+--------------+------------+-----------------+----"
         "--------+------------------+-------\n");
}

int confirmDelete() {
  int confirmDelete;
  do {
    puts("Confirmar remoção?");
    puts("0 - Continuar");
    puts("1 - Cancelar");
    printf("\nOpção: ");

    if (scanf("%d", &confirmDelete) != 1 ||
        (confirmDelete != 0 && confirmDelete != 1)) {
      clearBuffer();
      puts("\nOpção inválida.\n");
    } else {
      clearBuffer();
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

void cleanAllLists() {
  cleanupIntermediateTickets();
  cleanupIntermediateTicketsTypes();
  cleanupIntermediateUsers();
}

int calcMinHoras(DateTime dataIni, DateTime dataFim) {
  int retirarhora_em_min = ((24 - dataIni.hour) + dataFim.hour) * 60;
  int diferenca = dataIni.min - dataFim.min;
  if (diferenca < 0) {
    diferenca = abs(diferenca);
    retirarhora_em_min += diferenca;
  } else {
    retirarhora_em_min -= diferenca;
  }
  return retirarhora_em_min;
}

long calcMinDias(DateTime dataIni, DateTime dataFim) {
  long somaMin = 0;
  int diasRestMes = diasNoMes(dataIni.month, dataIni.year) - dataIni.day - 1;
  somaMin += (long)diasRestMes * 24 * 60;
  int diasInicioFim = dataFim.day - 1;
  somaMin += (long)diasInicioFim * 24 * 60;
  return somaMin;
}

long differenceInMinutes(DateTime dataIni, DateTime dataFim) {
  long somaMin = 0;

  // Caso Base -> Datas iguais retorna logo 0
  if (dataIni.year == dataFim.year && dataIni.month == dataFim.month &&
      dataIni.day == dataFim.day && dataIni.hour == dataFim.hour &&
      dataIni.min == dataFim.min)
    return 0;

  // Segundo caso -> Exatamente o mesmo dia porem em horas diferentes
  if (dataIni.year == dataFim.year && dataIni.month == dataFim.month &&
      dataIni.day == dataFim.day) {
    return ((dataFim.hour - dataIni.hour) * 60) + (dataFim.min - dataIni.min);
  }

  // Terceiro Caso -> Exatamente mesmo ano/mes porem dias e horas diferentes
  if (dataIni.year == dataFim.year && dataIni.month == dataFim.month) {
    int retirarhora_em_min = calcMinHoras(dataIni, dataFim);
    retirarhora_em_min += (dataFim.day - dataIni.day - 1) * 24 * 60;
    return retirarhora_em_min;
  }

  // Quarto Caso -> Mesmo ano mas mes diferente
  if (dataIni.year == dataFim.year) {
    somaMin += calcMinHoras(dataIni, dataFim);
    somaMin += calcMinDias(dataIni, dataFim);
    for (int m = dataIni.month + 1; m < dataFim.month; m++) {
      somaMin += (long)diasNoMes(m, dataIni.year) * 24 * 60;
    }
    return somaMin;
  }

  // Quinto Caso -> Anos diferentes
  somaMin += calcMinHoras(dataIni, dataFim);
  somaMin += calcMinDias(dataIni, dataFim);
  for (int m = dataIni.month + 1; m <= 12; m++) {
    somaMin += (long)diasNoMes(m, dataIni.year) * 24 * 60;
  }
  for (int m = 1; m < dataFim.month; m++) {
    somaMin += (long)diasNoMes(m, dataFim.year) * 24 * 60;
  }
  for (int a = dataIni.year + 1; a < dataFim.year; a++) {
    int diasAno = ((a % 4 == 0 && a % 100 != 0) || (a % 400 == 0)) ? 366 : 365;
    somaMin += (long)diasAno * 24 * 60;
  }
  return somaMin;
}

int charIsValid(char *str, size_t numMin, size_t numMax) {
  size_t lenght = strlen(str);

  if (lenght < numMin) {
    printf("\nInvalido, minimo %zu caracteres.", numMin);
    return -1;
  } else if (lenght > numMax) {
    printf("\nInvalido, maximo %zu caracteres.", numMax);
    return -1;
  } else {
    return 0;
  }
}