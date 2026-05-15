#include "funcoes.h"
#include <stdio.h>

void addHistory(ELEM_TICKET *ticket, HISTORY_INFO history)
{
  ELEM_HISTORY *newHistory = malloc(sizeof(ELEM_HISTORY));
  if (newHistory == NULL)
  {
    puts("Erro ao alocar memóra");
    return;
  }
  newHistory->data = history;
  newHistory->data.date = getCurrentDateTime();
  newHistory->next = ticket->history;
  ticket->history = newHistory;
}

void printHistory(int ticketId, ELEM_TICKET *headTickets)
{
  ELEM_TICKET *temp = headTickets;
  while (temp != NULL)
  {
    if (temp->data.id == ticketId)
    {
      ELEM_HISTORY *h = temp->history;
      if (h == NULL)
      {
        puts("Sem histórico para este ticket");
        return;
      }
      while (h != NULL)
      {
        printf("Data: %d/%d/%d %d:%d\n", h->data.date.day, h->data.date.month,
               h->data.date.year, h->data.date.hour, h->data.date.min);
        printf("Utilizador: %s\n", h->data.user);
        printf("Ação: %s\n", h->data.actionType);
        printf("Tecnico Anterior: %s\n", h->data.previousTechnician);
        printf("Tecnico Atual: %s\n", h->data.currentTechnician);
        printf("Estado anterior: %s\n", h->data.previousStatus);
        printf("Estado atual: %s\n", h->data.currentStatus);
        printf("Descrição: %s\n", h->data.description);
        puts("----------------------------------");
        h = h->next;
      }
      return;
    }
    temp = temp->next;
  }
  puts("Ticket não encontrado");
}