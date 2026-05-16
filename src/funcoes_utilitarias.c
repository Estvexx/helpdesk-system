#include "funcoes.h"
#include <stdio.h>
#include <string.h>


void clearBuffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void waitForKey()
{
  puts("\nCarregue numa tecla para continuar ...");
  getchar();
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
  case STATUS_IN_PROGRESS:
    strcpy(str, "Em Atend.");
    break;
  case STATUS_WAITING_USER:
    strcpy(str, "Esp. User");
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


void getPriority(int prioridade, char *texto)
{
  switch (prioridade)
  {
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

void tableHeaders()
{
  printf("\n%-5s | %-9s | %-12s | %-10s | %-15s | %-10s |%-16s\n", "ID", "Tipo", "Estado", "Prioridade", "Utilizador", "Tecnico", "Data Abertura");
  printf("------+-----------+--------------+------------+-----------------+------------+-----------------\n");
}

int confirmDelete()
{
  int confirmDelete;
  do
  {
    puts("Confirmar remoção?");
    puts("0 - Continuar");
    puts("1 - Cancelar");

    if (scanf("%d", &confirmDelete) != 1 || (confirmDelete != 0 && confirmDelete != 1))
    {
      clearBuffer();
      puts("Insira uma opção válida");
    }
    else
    {
      break;
    }

  } while (1);

  return confirmDelete;
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