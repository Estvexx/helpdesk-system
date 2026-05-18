#include "funcoes.h"
#include "stdio.h"

ELEM_TICKET_TYPE *headTicketsTypes = NULL;

void cleanupIntermediateTicketsTypes()
{
  cleanupTicketTypes(headTicketsTypes);
};

void getTypeUtil(int typeId, char *str)
{
  getType(typeId, str, headTicketsTypes);
}

int nextTicketTypeId = 1;

void seederTypes()
{
  TICKET_TYPE t;

  strcpy(t.name, "Hardware");
  createTicketType(t);

  strcpy(t.name, "Software");
  createTicketType(t);

  strcpy(t.name, "Rede");
  createTicketType(t);

  strcpy(t.name, "Acesso");
  createTicketType(t);

  strcpy(t.name, "Outro");
  createTicketType(t);
}

int createTicketType(TICKET_TYPE ticketType) //Necessário inserir no fim
{
  ELEM_TICKET_TYPE *newType = malloc(sizeof(ELEM_TICKET_TYPE));

  if (newType == NULL)
  {
    puts("Erro ao alocar memória para o tipo de ticket.");
    return -1;
  }

  newType->data = ticketType;
  newType->data.id = nextTicketTypeId;

  nextTicketTypeId++; 

  newType->next = NULL;

  if (headTicketsTypes == NULL)
  {
    headTicketsTypes = newType;
  }
  else
  {
    ELEM_TICKET_TYPE *temp = headTicketsTypes;

    while (temp->next != NULL) 
    {
      temp = temp->next;
    }

    temp->next = newType;
  }

  printf("Tipo de ticket '%s' criado com sucesso com o ID #%d!\n",
         newType->data.name, newType->data.id);

  return 0;
}

int deleteTicketType(int typeId, ELEM_TICKET *headTickets)
{
  int isConfirmed;

  if (headTicketsTypes == NULL)
  {
    puts("Nenhum tipo registado para remover.");
    return -1;
  }

  ELEM_TICKET_TYPE *temp = headTicketsTypes;
  ELEM_TICKET_TYPE *prev = NULL;

  while (temp != NULL && temp->data.id != typeId)
  {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL)
  {
    printf("Tipo #%d não encontrado!\n", typeId);
    return -1;
  }

  ELEM_TICKET *auxTicket;
  auxTicket = headTickets;

  while (auxTicket != NULL)
  {
    if (auxTicket->data.typeId == typeId)
    {
      puts("Impossível remover, existem tickets associados a este tipo.");
      return -1;
    }
    auxTicket = auxTicket->next;
  }

  isConfirmed = confirmDelete();

  if (isConfirmed == 1)
  {
    puts("Remoção cancelada.");
    return -1;
  }

  if (prev == NULL)
  {
    headTicketsTypes = temp->next;
  }
  else
  {
    prev->next = temp->next;
  }

  free(temp);

  printf("Tipo #%d removido com sucesso!\n", typeId);

  return 0;
}

int updateTicketType(int typeId)
{
  if (headTicketsTypes == NULL)
  {
    puts("Nenhum tipo de ticket registado para editar.");
    return -1;
  }

  ELEM_TICKET_TYPE *temp = headTicketsTypes;

  while (temp != NULL)
  {
    if (temp->data.id == typeId)
    {
      printf("\n=== TIPO ENCONTRADO ===\n");
      printf("ID: %d | Nome Atual: %s\n", temp->data.id, temp->data.name);

      printf("Novo nome: ");
      fgets(temp->data.name, sizeof(temp->data.name), stdin);
      temp->data.name[strcspn(temp->data.name, "\n")] = 0;

      printf("Tipo de ticket atualizado com sucesso!\n");
      return 0;
    }
    temp = temp->next;
  }

  printf("Tipo de ticket com o ID %d não encontrado!\n", typeId);
  return -1;
}

void listTicketTypes()
{
  if (headTicketsTypes == NULL)
  {
    puts("Nenhuma categoria registada.");
    return;
  }

  ELEM_TICKET_TYPE *temp = headTicketsTypes;
  int displayIndex = 1; // Id Visual

  while (temp != NULL)
  {
    printf("%d - %s\n", displayIndex, temp->data.name);

    temp = temp->next;
    displayIndex++;
  }
}

int getRealTypeId(int displayIndex)
{
  ELEM_TICKET_TYPE *temp = headTicketsTypes;
  int currentIndex = 1;

  while (temp != NULL)
  {

    if (currentIndex == displayIndex)
    {
      return temp->data.id;
    }
    currentIndex++;
    temp = temp->next;
  }

  return -1;
}

int getTicketTypeCount()
{
  ELEM_TICKET_TYPE *temp = headTicketsTypes;
  int count = 0;
  while (temp != NULL)
  {
    count++;
    temp = temp->next;
  }
  return count;
}
