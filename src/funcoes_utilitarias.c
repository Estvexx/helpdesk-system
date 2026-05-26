#include "funcoes.h"
#include "input/input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void waitForKey()
{
  puts("\nCarregue numa tecla para continuar ...");
  getchar();
}

DateTime getCurrentDateTime()
{
  DateTime d;
  time_t t = time(NULL);         // Converte para segundos o "t"
  struct tm *tm = localtime(&t); // converte para o tipo da data_hora do pc
  d.day = tm->tm_mday;           // Extrai o dia do mês (1-31)
  d.month = tm->tm_mon + 1;      // Extrai o mês (0-11)
  d.year = tm->tm_year + 1900;   // Extrai o ano (desde 1900)
  d.hour = tm->tm_hour;          // Extrai a hora (0-23) da estrutura tm
  d.min = tm->tm_min;            // Extrai os minutos (0-59)
  return d;
}

// Retorna 1 se d1 > d2, 0 se igual, -1 se d1 < d2
int compareDates(DateTime d1, DateTime d2)
{
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

void getStatus(int status, char *str)
{
  switch (status)
  {
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

void getPriority(int priority, char *str)
{
  switch (priority)
  {
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

int getSLA(int priority)
{
  switch (priority)
  {
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

void tableHeaders()
{
  printf("\n%-5s | %-18s | %-20s | %-10s | %-18s | %6s | %-16s | %-5s\n", "ID",
         "Tipo", "Estado", "Prioridade", "Utilizador", "Tecnico",
         "Data Abertura", "SLA");
  printf("------+--------------------+----------------------+------------+-----"
         "------"
         "---------+----"
         "-----+------------------+-------\n");
}

int confirmDelete()
{
  puts("Confirmar remoção?");
  puts("0 - Continuar");
  puts("1 - Cancelar");
  return readIntRange("\nOpção: ", 0, 1);
}

void getType(int typeId, char *str, ELEM_TICKET_TYPE *headTicketsTypes)
{
  ELEM_TICKET_TYPE *temp = headTicketsTypes;

  while (temp != NULL)
  {
    if (temp->data.id == typeId)
    {
      strcpy(str, temp->data.name);
      return;
    }
    temp = temp->next;
  }
  strcpy(str, "???");
}

DateTime addTimeToDateTime(DateTime dt, int hours, int minutes)
{
  DateTime result = dt;

  // EX -> Hora atual(result) = 12:30
  // Exemplo base -> se chegar 100 minutos via parametro somo aos minutos do
  // result 12:130
  result.min += minutes;
  result.hour += hours;

  // Aqui enquanto for maior ou igual a 60 subtraio 60 e aumenta 1 hora
  while (result.min >= 60)
  {
    result.min -= 60;
    result.hour++;
  }

  // O mesmo para as horas, se tiver mais de 24h passo para o dia seguite
  while (result.hour >= 24)
  {
    result.hour -= 24;
    result.day++;
  }

  // Já tá feita a logica para os meses 31/30 direitinho
  while (result.day > daysInMonth(result.month, result.year))
  {
    result.day -= daysInMonth(result.month, result.year);
    result.month++;

    if (result.month > 12)
    {
      result.month = 1;
      result.year++;
    }
  }

  return result;
}

int daysInMonth(int month, int year)
{
  switch (month)
  {
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

void cleanAllLists()
{
  cleanupIntermediateTickets();
  cleanupIntermediateTicketsTypes();
  cleanupIntermediateUsers();
}

int calculateHoursMinutes(DateTime startDate, DateTime endDate)
{
  int minutesToRemove = ((24 - startDate.hour) + endDate.hour) * 60;
  int minuteDifference = startDate.min - endDate.min;
  if (minuteDifference < 0)
  {
    minuteDifference = abs(minuteDifference);
    minutesToRemove += minuteDifference;
  }
  else
  {
    minutesToRemove -= minuteDifference;
  }
  return minutesToRemove;
}

long calculateDaysMinutes(DateTime startDate, DateTime endDate)
{
  long totalMinutes = 0;
  int remainingMonthDays =
      daysInMonth(startDate.month, startDate.year) - startDate.day - 1;
  totalMinutes += (long)remainingMonthDays * 24 * 60;
  int startEndDays = endDate.day - 1;
  totalMinutes += (long)startEndDays * 24 * 60;
  return totalMinutes;
}

long differenceInMinutes(DateTime startDate, DateTime endDate)
{
  long totalMinutes = 0;

  // Caso Base -> Datas iguais retorna logo 0
  if (startDate.year == endDate.year && startDate.month == endDate.month &&
      startDate.day == endDate.day && startDate.hour == endDate.hour &&
      startDate.min == endDate.min)
    return 0;

  // Segundo caso -> Exatamente o mesmo dia porem em horas diferentes
  if (startDate.year == endDate.year && startDate.month == endDate.month &&
      startDate.day == endDate.day)
  {
    return ((endDate.hour - startDate.hour) * 60) +
           (endDate.min - startDate.min);
  }

  // Terceiro Caso -> Exatamente mesmo ano/mes porem dias e horas diferentes
  if (startDate.year == endDate.year && startDate.month == endDate.month)
  {
    int minutesToRemove = calculateHoursMinutes(startDate, endDate);
    minutesToRemove += (endDate.day - startDate.day - 1) * 24 * 60;
    return minutesToRemove;
  }

  // Quarto Caso -> Mesmo ano mas mes diferente
  if (startDate.year == endDate.year)
  {
    totalMinutes += calculateHoursMinutes(startDate, endDate);
    totalMinutes += calculateDaysMinutes(startDate, endDate);
    for (int month = startDate.month + 1; month < endDate.month; month++)
    {
      totalMinutes += (long)daysInMonth(month, startDate.year) * 24 * 60;
    }
    return totalMinutes;
  }

  // Quinto Caso -> Anos diferentes
  totalMinutes += calculateHoursMinutes(startDate, endDate);
  totalMinutes += calculateDaysMinutes(startDate, endDate);
  for (int month = startDate.month + 1; month <= 12; month++)
  {
    totalMinutes += (long)daysInMonth(month, startDate.year) * 24 * 60;
  }
  for (int month = 1; month < endDate.month; month++)
  {
    totalMinutes += (long)daysInMonth(month, endDate.year) * 24 * 60;
  }
  for (int year = startDate.year + 1; year < endDate.year; year++)
  {
    int daysInYear =
        ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 366 : 365;
    totalMinutes += (long)daysInYear * 24 * 60;
  }
  return totalMinutes;
}