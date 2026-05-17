#include "funcoes.h"
#include <stdio.h>

void cleanupUsers(ELEM_USER *headUsers) {
  ELEM_USER *temp = headUsers;
  
  while (temp != NULL) {
    ELEM_USER *next = temp->next;
    free(temp);
    temp = next;
  }

  headUsers = NULL;
}

void cleanupTickets(ELEM_TICKET *headTickets) {
  ELEM_TICKET *temp = headTickets;
  while (temp != NULL) {
    ELEM_TICKET *next = temp->next;

    ELEM_HISTORY *history = temp->history;
    while (history != NULL) {
      ELEM_HISTORY *next = history->next;
      free(history);
      history = next;
    }

    free(temp);
    temp = next;
  }
  headTickets = NULL;
}

void cleanupTicketTypes(ELEM_TICKET_TYPE *headTicketsTypes) {
  ELEM_TICKET_TYPE *temp = headTicketsTypes;
  while (temp != NULL) {
    ELEM_TICKET_TYPE *next = temp->next;
    free(temp);
    temp = next;
  }
  headTicketsTypes = NULL;
}