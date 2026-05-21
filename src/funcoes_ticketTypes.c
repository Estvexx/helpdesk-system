#include "funcoes.h"
#include "stdio.h"
#include "input/input.h"

ELEM_TICKET_TYPE *headTicketsTypes = NULL;

void cleanupIntermediateTicketsTypes() {
  cleanupTicketTypes(headTicketsTypes);
};

void getTypeUtil(int typeId, char *str) {
  getType(typeId, str, headTicketsTypes);
}

int nextTicketTypeId = 1;

void inicializarIds_TicketType() {
  // AQUI NAO PRECISO DE VERIFICAR SE A LSITA ESTA VAZIA
  ELEM_TICKET_TYPE *temp = headTicketsTypes;
  while (temp != NULL) {
    if (temp->data.id > nextTicketTypeId) {
      nextTicketTypeId = temp->data.id + 1;
    }
    temp = temp->next;
  }
}

void seederTypes() {
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

int createTicketType(TICKET_TYPE ticketType) // Necessário inserir no fim
{
  ELEM_TICKET_TYPE *newType = malloc(sizeof(ELEM_TICKET_TYPE));

  if (newType == NULL) {
    puts("\nERRO: Falha ao alocar memória para o tipo de ticket.");
    return -1;
  }

  newType->data = ticketType;
  newType->data.id = nextTicketTypeId;
  nextTicketTypeId++;

  newType->next = NULL;

  if (headTicketsTypes == NULL) {
    headTicketsTypes = newType;
  } else {
    ELEM_TICKET_TYPE *temp = headTicketsTypes;

    while (temp->next != NULL) {
      temp = temp->next;
    }

    temp->next = newType;
  }

  return 0;
}

int deleteTicketType(int typeId, ELEM_TICKET *headTickets) {
  int isConfirmed;

  if (headTicketsTypes == NULL) {
    puts("\nERRO: Nenhum tipo registado para remover.");
    return -1;
  }

  ELEM_TICKET_TYPE *temp = headTicketsTypes;
  ELEM_TICKET_TYPE *prev = NULL;

  while (temp != NULL && temp->data.id != typeId) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL) {
    puts("\nERRO: Tipo não encontrado!");
    return -1;
  }

  ELEM_TICKET *auxTicket;
  auxTicket = headTickets;

  while (auxTicket != NULL) {
    if (auxTicket->data.typeId == typeId) {
      puts("\nERRO: Impossível remover, existem tickets associados a este tipo.");
      return -1;
    }
    auxTicket = auxTicket->next;
  }

  isConfirmed = confirmDelete();

  if (isConfirmed == 1) {
    puts("\nRemoção cancelada.");
    return -1;
  }

  if (prev == NULL) {
    headTicketsTypes = temp->next;
  } else {
    prev->next = temp->next;
  }

  free(temp);

  puts("Tipo removido com sucesso!");

  return 0;
}

int updateTicketType(int typeId) {
  if (headTicketsTypes == NULL) {
    puts("\nERRO: Nenhum tipo de ticket registado para editar.");
    return -1;
  }

  ELEM_TICKET_TYPE *temp = headTicketsTypes;

  while (temp != NULL) {
    if (temp->data.id == typeId) {
      printf("ID: %d | Nome Atual: %s\n", temp->data.id, temp->data.name);

      do {
        readString("Novo nome: ", temp->data.name, sizeof(temp->data.name));
      } while (charIsValid(temp->data.name, 3, sizeof(temp->data.name) - 1) ==
               -1);

      return 0;
    }
    temp = temp->next;
  }

  printf("\nERRO: Tipo de ticket com o ID %d não encontrado!\n", typeId);
  return -1;
}

void listTicketTypes() {
  if (headTicketsTypes == NULL) {
    puts("Nenhuma categoria registada.");
    return;
  }

  ELEM_TICKET_TYPE *temp = headTicketsTypes;
  int displayIndex = 1; // Id Visual

  while (temp != NULL) {
    printf("%d - %s\n", displayIndex, temp->data.name);

    temp = temp->next;
    displayIndex++;
  }
}

int getRealTypeId(int displayIndex) {
  ELEM_TICKET_TYPE *temp = headTicketsTypes;
  int currentIndex = 1;

  while (temp != NULL) {

    if (currentIndex == displayIndex) {
      return temp->data.id;
    }
    currentIndex++;
    temp = temp->next;
  }

  return -1;
}

int getTicketTypeCount() {
  ELEM_TICKET_TYPE *temp = headTicketsTypes;
  int count = 0;
  while (temp != NULL) {
    count++;
    temp = temp->next;
  }
  return count;
}

int saveCategoryTypesToFile(const char *filename) {
  FILE *fp = fopen(filename, "wb");
  if (fp == NULL) {
    printf("\nERRO: Não foi possível guardar categorias\n");
    waitForKey();
    return -1;
  }

  int count = getTicketTypeCount();
  fwrite(&count, sizeof(int), 1, fp);

  ELEM_TICKET_TYPE *temp = headTicketsTypes;
  while (temp != NULL) {
    fwrite(&(temp->data), sizeof(TICKET_TYPE), 1, fp);
    temp = temp->next;
  }

  fclose(fp);
  printf("%d categorias guardadas em: %s\n", count, filename);
  return 0;
}

int loadCategoryTypesFromFile(const char *filename) {
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("Ficheiro %s não encontrado\n", filename);
    waitForKey();
    return -1;
  }

  int count;
  fread(&count, sizeof(int), 1, fp);

  for (int i = 0; i < count; i++) {
    ELEM_TICKET_TYPE *new = malloc(sizeof(ELEM_TICKET_TYPE));
    if (new == NULL) {
      fclose(fp);
      return -1;
    }

    fread(&(new->data), sizeof(TICKET_TYPE), 1, fp);

    // Adicionar ao final (para manter ordem)
    if (headTicketsTypes == NULL) {
      new->next = NULL;
      headTicketsTypes = new;
    } else {
      ELEM_TICKET_TYPE *temp = headTicketsTypes;
      while (temp->next != NULL) {
        temp = temp->next;
      }
      temp->next = new;
      new->next = NULL;
    }
  }

  fclose(fp);
  printf("%d categorias carregadas de: %s\n", count, filename);
  return 0;
}
