#include "funcoes.h"
#include "input/input.h"
#include "ui/ui.h"
#include <stdio.h>
#include <string.h>

#define REPORTS_PATH "reports\\"

ELEM_TICKET *headTickets = NULL;

int createStatsToReports(char *fileName, char *fileHeader, ReportStats stats);

void cleanupIntermediateTickets() { cleanupTickets(&headTickets); }

// Retorna 0 se existem tickets, -1 se a lista estiver vazia
int hasTickets(void) {
  if (headTickets == NULL) {
    puts("Nenhum ticket registado no sistema.");
    return -1;
  }
  return 0;
}

// ========================= HISTORY
void printTicketHistory(int ticketId) { printHistory(ticketId, headTickets); }
int deleteType(int typeId) { return deleteTicketType(typeId, headTickets); }

int nextTicketId = 1;

void initializeTicketIds() {
  // AQUI NAO PRECISO DE VERIFICAR SE A LSITA ESTA VAZIA
  ELEM_TICKET *temp = headTickets;
  while (temp != NULL) {
    if (temp->data.id > nextTicketId) {
      nextTicketId = temp->data.id + 1;
    }
    temp = temp->next;
  }
}

int createTicket(TICKET_INFO ticket) {
  ELEM_TICKET *new = malloc(sizeof(ELEM_TICKET));
  if (new == NULL) {
    puts("\nERRO: Falha ao alocar memória");
    return -1;
  }

  new->data = ticket;
  new->data.id = nextTicketId;
  nextTicketId++;
  new->data.technicianId = -1; // deixar -1 porque nao tem tecnico associado
  new->data.status = STATUS_OPEN;
  new->data.openedAt = getCurrentDateTime();
  /* new->data.closedAt.day = 0;
  new->data.closedAt.month = 0;
  new->data.closedAt.year = 0;
  new->data.closedAt.hour = 0;
  new->data.closedAt.min = 0; */
  new->data.closedAt = (DateTime){0};
  new->data.estimatedConclusion = (DateTime){0};
  strcpy(new->data.actions, "");
  strcpy(new->data.tools, "");
  new->history = NULL;

  new->next = headTickets;
  headTickets = new;

  return 0;
}

int getTicketCount() {
  if (hasTickets() == -1)
    return 0;

  int count = 0;
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL) {
    count++;
    temp = temp->next;
  }
  return count;
}

int deleteTicket(int id) {
  if (hasTickets() == -1)
    return -1;

  ELEM_TICKET *temp = headTickets;
  ELEM_TICKET *anterior = NULL;

  while (temp != NULL) {
    if (temp->data.id == id) {
      printInfosTicket(temp->data);
      int confirm = confirmDelete();

      if (confirm == 1) {
        puts("\nERRO: Remoção cancelada");
        return -1;
      }
      if (anterior == NULL) {
        headTickets = temp->next; // É o primeiro elemento
      } else {
        anterior->next = temp->next; // Está no meio ou no fim
      }

      // Libertar histórico
      ELEM_HISTORY *histTemp = temp->history;
      while (histTemp != NULL) {
        ELEM_HISTORY *aux = histTemp;
        histTemp = histTemp->next;
        free(aux); // libertar memoria do historico
      }
      free(temp); // libertar memoria do ticket
      return 0;   // Retorno sucesso
    }
    anterior = temp;
    temp = temp->next;
  }
  return -1;
}

int updateTicket(int ticketId, int logged_userId) {
  if (hasTickets() == -1)
    return -1;

  int option, running = 1;

  ELEM_TICKET *temp = headTickets;

  while (temp != NULL) {
    if (temp->data.id == ticketId) {
      printInfosTicket(temp->data);

      do {
        system("cls");
        Navbar();
        Title("EDITAR TICKET", "📝");
        printInfosTicket(temp->data);
        puts("\n\n1 - Alterar Tipo");
        puts("2 - Alterar Descrição");
        puts("3 - Alterar Prioridade");
        puts("4 - Alterar Estado");
        puts("0 - Voltar");
        option = readIntRange("\nOpção: ", 0, 4);

        switch (option) {
        case 1: {
          // Alterar Tipo
          system("cls");
          Navbar();
          Title("EDITAR TICKET", "📝");
          printInfosTicket(temp->data);

          int realId, displayId;
          do {
            system("cls");
            Navbar();
            Title("EDITAR TICKET", "📝");
            printInfosTicket(temp->data);
            printf("\nNovo tipo:\n");
            listTicketTypes();
            displayId = readIntRange("\nOpção: ", 1, getTicketTypeCount());
            realId = getRealTypeId(displayId);
            if (realId == -1) {
              puts("Opção inválida");
            } else {
              temp->data.typeId = realId;
              puts("\nSUCESSO: Tipo atualizado");
              break;
            }
          } while (1);

          break;
        }
        case 2:
          // Alterar Descrição
          system("cls");
          Navbar();
          Title("EDITAR TICKET", "📝");
          printInfosTicket(temp->data);
          do {
            readString("Descrição: ", temp->data.description,
                       sizeof(temp->data.description));
          } while (validateStringLength(temp->data.description, 5,
                                        sizeof(temp->data.description) - 1) ==
                   -1);
          puts("\nSUCESSO: Descrição atualizada");
          break;
        case 3:
          // Alterar Prioridade
          system("cls");
          Navbar();
          Title("EDITAR TICKET", "📝");
          do {
            printInfosTicket(temp->data);
            temp->data.priority = readIntRange(
                "\nPrioridade (1-Baixa, 2-Media, 3-Alta, 4-Critica): ", 1, 4);
            puts("\nSUCESSO: Prioridade atualizada");
            break;
          } while (1);

          break;
        case 4:
          system("cls");
          Navbar();
          Title("EDITAR TICKET", "📝");
          updateTicketStatus(ticketId, logged_userId);
          break;
        case 0:
          running = 0;
          return 0;
        default:
          system("cls");
          Navbar();
          Title("EDITAR TICKET", "📝");
          puts("Opção Inválida!");
          break;
        }
        waitForKey();
      } while (running);

      return 0;
    }

    temp = temp->next;
  }
  printf("\nERRO: Ticket não encontrado!\n");
  return -1;
}

int listAllTickets() {
  if (hasTickets() == -1)
    return -1;

  ELEM_TICKET *temp = headTickets;
  tableHeaders();
  while (temp != NULL) {
    printInfoFormatTable(temp->data);

    temp = temp->next;
  }
  puts(""); // Só para dar enter
  return 0;
}

int showTicketById(int id) {
  if (hasTickets() == -1)
    return -1;

  ELEM_TICKET *temp = headTickets;

  while (temp != NULL) {
    if (temp->data.id == id) {
      printInfosTicket(temp->data);
      return 0;
    }
    temp = temp->next;
  }
  return -1;
}

void showTicketByTechnician(int id) {
  if (hasTickets() == -1)
    return;

  int found = 0;

  ELEM_TICKET *temp = headTickets;

  tableHeaders();

  while (temp != NULL) {
    if (temp->data.technicianId == id) {
      printInfoFormatTable(temp->data);
      found = 1;
    }
    temp = temp->next;
  }
  if (!found) {
    puts("Nenhum atribuido a si");
  }
  printf("\n");
}

void listPendentTickets() {
  if (hasTickets() == -1)
    return;

  ELEM_TICKET *temp = headTickets;

  tableHeaders();
  while (temp != NULL) {
    if (temp->data.status == STATUS_OPEN) {
      printInfoFormatTable(temp->data);
    }
    temp = temp->next;
  }
  puts(""); // estilo visual enter
}

void showTicketPendentByTechnician(int id) {
  if (hasTickets() == -1)
    return;
  int found = 0;
  ELEM_TICKET *temp = headTickets;

  tableHeaders();
  while (temp != NULL) {
    if (temp->data.technicianId == id &&
        temp->data.status == STATUS_WAITING_USER) {
      printInfoFormatTable(temp->data);
      found = 1;
    }
    temp = temp->next;
  }
  if (!found) {
    puts("Nenhum atribuido a si");
  }
  puts("");
}

int assignTechnician(int ticket_id, int technicianId) {
  if (hasTickets() == -1)
    return -1;
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL) {
    if (temp->data.id == ticket_id) {
      if (isTechnicianValidated(technicianId) == -1) {
        puts("\nTecnico não encontrado");
        return -1;
      }
      if (isTechnicianValidated(technicianId) == 0) {
        puts("\nTecnico deve ser validado");
        return -1;
      }

      // Se o status nao for open nao pode atribuir tenico
      if (temp->data.status != STATUS_OPEN) {
        puts("\nTicket ja esta em atendimento ou fechado. Impossivel alterar "
             "tecnico");
        return -1;
      }
      HISTORY_INFO h;

      h.date = getCurrentDateTime();
      strcpy(h.user, "admin");
      strcpy(h.actionType, "ATRIBUICAO DE TICKET A TECNICO");
      if (temp->data.technicianId == -1) {
        strcpy(h.previousTechnician, "N/D");
      } else {
        snprintf(h.previousTechnician, 30, "%d", temp->data.technicianId);
      }
      snprintf(h.currentTechnician, 30, "%d", technicianId);
      getStatus(temp->data.status, h.previousStatus);
      temp->data.technicianId = technicianId;

      if (temp->data.status == STATUS_OPEN) {
        temp->data.status = STATUS_WAITING_USER;
      }
      getStatus(temp->data.status, h.currentStatus);
      snprintf(h.description, 300, "\nTicket atribuido ao tecnico #%d",
               technicianId);

      // Adicionar histórico
      addHistory(temp, h);

      return 0;
    }
    temp = temp->next;
  }

  puts("Ticket nao encontrado!");
  return -1;
}

// Listar Tickets por FILTROS
void listTicketsByFilter(int value, char filter) {
  if (hasTickets() == -1)
    return;

  ELEM_TICKET *temp = headTickets;
  int found = 0;

  tableHeaders();

  while (temp != NULL) {
    int match = 0;

    switch (filter) {
    case 'S':
      match = (temp->data.status == value);
      break; // se os resultados forem iguais match = 1
    case 'P':
      match = (temp->data.priority == value);
      break; // se os resultados forem iguais match = 1
    case 'T':
      match = (temp->data.typeId == value);
      break; // se os resultados forem iguais match = 1
    }

    if (match) {
      printInfoFormatTable(temp->data);
      found = 1;
    }
    temp = temp->next;
  }

  if (!found)
    puts("Nenhum ticket encontrado com esse filtro");
  puts("");
}

// Ordenar Tickets
void sortTicketsByPriority() {
  if (headTickets == NULL || headTickets->next == NULL) {
    puts("Operação impossivel de executar");
    return;
  }

  ELEM_TICKET *limit = NULL;
  int swapped = 1;

  while (swapped) {
    swapped = 0;
    ELEM_TICKET *current = headTickets;

    while (current->next != limit) {
      if (current->data.priority < current->next->data.priority) {
        TICKET_INFO temp = current->data;
        current->data = current->next->data;
        current->next->data = temp;
        swapped = 1;

        // Aqui levo o historico com ele tambem, nao ordeno so a info do ticket
        ELEM_HISTORY *tempHistory = current->history;
        current->history = current->next->history;
        current->next->history = tempHistory;
      }
      current = current->next;
    }
    limit = current;
  }
  puts("Ordenado com sucesso");
}

void sortTicketsByDate() {
  if (headTickets == NULL || headTickets->next == NULL) {
    puts("Operação impossivel de executar");
    return;
  }

  ELEM_TICKET *limit = NULL;
  int swapped = 1;

  while (swapped) {
    swapped = 0;
    ELEM_TICKET *current = headTickets;

    while (current->next != limit) {
      if (compareDates(current->data.openedAt, current->next->data.openedAt) >
          0) {
        TICKET_INFO temp = current->data;
        current->data = current->next->data;
        current->next->data = temp;

        // Aqui levo o historico com ele tambem, nao ordeno so a info do ticket
        ELEM_HISTORY *tempHistory = current->history;
        current->history = current->next->history;
        current->next->history = tempHistory;
        swapped = 1;
      }
      current = current->next;
    }
    limit = current;
  }
  puts("Ordenado com sucesso");
}

void sortTicketsById() {
  if (headTickets == NULL || headTickets->next == NULL) {
    puts("Operação impossivel de executar");
    return;
  }

  ELEM_TICKET *limit = NULL;
  int swapped = 1;

  while (swapped) {
    swapped = 0;
    ELEM_TICKET *current = headTickets;

    while (current->next != limit) {
      if (current->data.id > current->next->data.id) {
        TICKET_INFO temp = current->data;
        current->data = current->next->data;
        current->next->data = temp;

        // Aqui levo o historico com ele tambem, nao ordeno so a info do ticket
        ELEM_HISTORY *tempHistory = current->history;
        current->history = current->next->history;
        current->next->history = tempHistory;

        swapped = 1;
      }
      current = current->next;
    }
    limit = current;
  }
  puts("Ordenado com sucesso");
}

void sortTicketsByTechnician() {
  if (headTickets == NULL || headTickets->next == NULL) {
    puts("Operação impossivel de executar");
    return;
  }

  ELEM_TICKET *limit = NULL;
  int swapped = 1;

  while (swapped) {
    swapped = 0;
    ELEM_TICKET *current = headTickets;

    while (current->next != limit) {
      if (current->data.technicianId > current->next->data.technicianId) {
        TICKET_INFO temp = current->data;
        current->data = current->next->data;
        current->next->data = temp;

        // Aqui levo o historico com ele tambem, nao ordeno so a info do ticket
        ELEM_HISTORY *tempHistory = current->history;
        current->history = current->next->history;
        current->next->history = tempHistory;

        swapped = 1;
      }
      current = current->next;
    }
    limit = current;
  }
  puts("Ordenado com sucesso");
}

int updateTicketStatus(int ticket_id, int logged_userId) {
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL && temp->data.id != ticket_id) {
    temp = temp->next;
  }

  if (temp == NULL) {
    printf("Ticket #%d não encontrado.\n", ticket_id);
    return -1;
  }

  if (temp->data.technicianId != logged_userId &&
      isAdmin(logged_userId) == -1) {
    puts("Apenas é possível alterar o estado dos tickets a si atribuidos.");
    return -1;
  }

  char strStatus[30];
  getStatus(temp->data.status, strStatus);
  printf("Estado atual: %d - %s\n", temp->data.status, strStatus);

  if (temp->data.status == STATUS_OPEN) {
    puts("Ticket em estado Aberto. Caso o ticket esteja atribuido a si utilize "
         "'Aceitar Ticket' primeiro");
    return -1;
  }
  if (temp->data.status == STATUS_CLOSED) {
    puts("Ticket já está fechado. Não pode ser alterado");
    return -1;
  }

  int newStatus;
  char acao[20];

  if (temp->data.status == STATUS_IN_PROGRESS) {
    newStatus = STATUS_RESOLVED;
    strcpy(acao, "RESOLUCAO");
  } else if (temp->data.status == STATUS_RESOLVED) {
    newStatus = STATUS_CLOSED;
    strcpy(acao, "FECHO");
  } else {
    printf("Estado '%s' não permite alteração.\n", strStatus);
    return -1;
  }

  char novoStr[30];
  getStatus(newStatus, novoStr);
  printf("Alterar estado para '%s'?\n", novoStr);
  int confirm = readIntRange("1-Sim / 0-Não: ", 0, 1);

  if (confirm != 1) {
    puts("Operação cancelada");
    return -1;
  }

  // ========== PREENCHER DADOS E HISTÓRICO ==========
  HISTORY_INFO h;
  h.date = getCurrentDateTime();
  snprintf(h.user, MAX_STR, "%d", logged_userId);
  strcpy(h.actionType, acao);
  snprintf(h.previousTechnician, 30, "%d", temp->data.technicianId);
  snprintf(h.currentTechnician, 30, "%d", temp->data.technicianId);
  getStatus(temp->data.status, h.previousStatus);
  getStatus(newStatus, h.currentStatus);

  if (newStatus == STATUS_RESOLVED) {
    char solucao[500];
    do {
      readString("Descreva a solução aplicada: ", solucao, sizeof(solucao));
    } while (validateStringLength(solucao, 5, sizeof(solucao) - 1) == -1);
    temp->data.closedAt = getCurrentDateTime();
    snprintf(h.description, 600, "Ticket resolvido. Solução: %s", solucao);
  } else if (newStatus == STATUS_CLOSED) {
    do {
      readString("Descreva as ações realizadas: ", temp->data.actions,
                 sizeof(temp->data.actions));
    } while (validateStringLength(temp->data.actions, 5,
                                  sizeof(temp->data.actions) - 1) == -1);

    do {
      readString("Ferramentas utilizadas: ", temp->data.tools,
                 sizeof(temp->data.tools));
    } while (validateStringLength(temp->data.tools, 3,
                                  sizeof(temp->data.tools) - 1) == -1);

    snprintf(h.description, 800, "Ticket fechado. Ações: %s | Ferramentas: %s",
             temp->data.actions, temp->data.tools);
    temp->data.closedAt = getCurrentDateTime();
  }

  temp->data.status = newStatus;

  addHistory(temp, h);

  printf("Estado alterado para '%s'.\n", novoStr);

  return 0;
}

int acceptTicket(int tecnicoId, int ticketId) {
  if (hasTickets() == -1)
    return -1;

  ELEM_TICKET *temp = headTickets;

  while (temp != NULL) {
    // É necessario verificar se o ticket está associado a esse mesmo tecnico
    if (temp->data.id == ticketId) {
      if (temp->data.technicianId != tecnicoId) {
        puts("Este ticket nao esta atribuido a si");
        return -1;
      }

      // Verificar se o ticket ainda está a espera de repstoa
      if (temp->data.status != STATUS_WAITING_USER) {
        puts("Este ticket ja foi aceite ou ja nao esta atribuido a si");
        return -1;
      }

      int horas, minutos;
      puts("Tempo estimado de resolucao (horas e minutos):");
      horas = readIntRange("Horas: ", 0, 200);
      minutos = readIntRange("Minutos: ", 0, 59);

      temp->data.status = STATUS_IN_PROGRESS;

      // Calcular data prevista
      temp->data.estimatedConclusion =
          addTimeToDateTime(getCurrentDateTime(), horas, minutos);

      printf("\nData prevista de conclusao: %02d/%02d/%04d %02d:%02d\n",
             temp->data.estimatedConclusion.day,
             temp->data.estimatedConclusion.month,
             temp->data.estimatedConclusion.year,
             temp->data.estimatedConclusion.hour,
             temp->data.estimatedConclusion.min);

      HISTORY_INFO h;
      h.date = getCurrentDateTime();
      strcpy(h.user, "tecnico");
      strcpy(h.actionType, "ACEITACAO DO TECNICO");
      snprintf(h.currentTechnician, 30, "%d", tecnicoId);
      snprintf(h.previousTechnician, 30, "%d", tecnicoId);
      getStatus(STATUS_WAITING_USER, h.previousStatus);
      getStatus(STATUS_IN_PROGRESS, h.currentStatus);
      snprintf(
          h.description, 300,
          "Ticket aceite pelo tecnico #%d. Previsao: %02d/%02d/%04d %02d:%02d",
          tecnicoId, temp->data.estimatedConclusion.day,
          temp->data.estimatedConclusion.month,
          temp->data.estimatedConclusion.year,
          temp->data.estimatedConclusion.hour,
          temp->data.estimatedConclusion.min);

      addHistory(temp, h);
      return 0;
    }
    temp = temp->next;
  }

  puts("Ticket nao encontrado");
  return -1;
}

int delegateTicket(int ticket_id, int logged_userId) {
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL && temp->data.id != ticket_id) {
    temp = temp->next;
  }

  if (temp == NULL) {
    printf("Ticket #%d não encontrado.\n", ticket_id);
    return -1;
  }
  // Verificar se é o user da sessão que relamente está a fazer a delegaçao
  if (temp->data.technicianId != logged_userId) {
    puts("Este ticket não está atribuído a si.");
    return -1;
  }

  if (temp->data.status == STATUS_CLOSED) {
    puts("Não é possível delegar um ticket fechado.");
    return -1;
  }

  int newTechnicianId;
  listAllTechnicians();
  newTechnicianId = readInt("ID do técnico para delegar: ");

  if (newTechnicianId == logged_userId) {
    puts("Não pode delegar um ticket para si próprio.");
    return -1;
  }

  if (isTechnicianValidated(newTechnicianId) == -1) {
    printf("Técnico #%d não encontrado.\n", newTechnicianId);
    return -1;
  }
  if (isTechnicianValidated(newTechnicianId) == 0) {
    printf("Técnico #%d ainda não foi validado.\n", newTechnicianId);
    return -1;
  }

  char motivo[300];
  do {
    readString("Motivo da delegação: ", motivo, sizeof(motivo));
  } while (validateStringLength(motivo, 3, sizeof(motivo) - 1) == -1);

  HISTORY_INFO h;
  h.date = getCurrentDateTime();
  snprintf(h.user, MAX_STR, "%d", logged_userId);
  strcpy(h.actionType, "DELEGACAO");
  snprintf(h.previousTechnician, 30, "%d", temp->data.technicianId);
  snprintf(h.currentTechnician, 30, "%d", newTechnicianId);
  getStatus(temp->data.status, h.previousStatus);
  getStatus(temp->data.status, h.currentStatus);
  snprintf(h.description, 500, "Ticket delegado ao técnico #%d. Motivo: %s",
           newTechnicianId, motivo);

  temp->data.technicianId = newTechnicianId;

  addHistory(temp, h);

  return 0;
}

int addComment(int ticket_id, int logged_userId) {
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL && temp->data.id != ticket_id) {
    temp = temp->next;
  }

  if (temp == NULL) {
    printf("Ticket #%d não encontrado.\n", ticket_id);
    return -1;
  }

  if (temp->data.technicianId != logged_userId) {
    puts("Este ticket não está atribuído a si");
    return -1;
  }

  if (temp->data.status == STATUS_CLOSED) {
    puts("Não é possível comentar um ticket fechado");
    return -1;
  }

  char comentario[300];
  do {
    readString("Comentário: ", comentario, sizeof(comentario));
  } while (validateStringLength(comentario, 3, sizeof(comentario) - 1) == -1);

  // Histórico
  HISTORY_INFO h;
  h.date = getCurrentDateTime();
  snprintf(h.user, MAX_STR, "%d", logged_userId);
  strcpy(h.actionType, "COMENTARIO");
  snprintf(h.previousTechnician, 30, "%d", temp->data.technicianId);
  snprintf(h.currentTechnician, 30, "%d", temp->data.technicianId);
  getStatus(temp->data.status, h.previousStatus);
  getStatus(temp->data.status, h.currentStatus);
  strcpy(h.description, comentario);

  addHistory(temp, h);

  return 0;
}

void averageTimePerTechnician() {
  if (hasTickets() == -1)
    return;

  // Projeto académico, para testar nao vamos ate 100 users
  long minutos[100] = {0};
  int contagem[100] = {0};

  ELEM_TICKET *temp = headTickets;

  while (temp != NULL) {
    if ((temp->data.status == STATUS_RESOLVED ||
         temp->data.status == STATUS_CLOSED) &&
        temp->data.technicianId != -1) {

      int id = temp->data.technicianId;
      long diff = differenceInMinutes(temp->data.openedAt, temp->data.closedAt);

      minutos[id] += diff;
      contagem[id]++;
    }
    temp = temp->next;
  }

  printf("\n--- TEMPO MEDIO DE RESOLUCAO POR TECNICO ---\n");
  printf("%-10s | %-15s | %-10s\n", "Tecnico", "Tempo Medio", "Tickets");
  printf("-----------+-----------------+------------\n");

  int found = 0;
  for (int i = 0; i < 100; i++) {
    if (contagem[i] > 0) {
      float media = (float)minutos[i] / contagem[i];
      float horas = media / 60;
      printf("#%-9d | %.1f horas       | %d\n", i, horas, contagem[i]);
      found = 1;
    }
  }

  if (!found) {
    puts("Nenhum ticket resolvido");
  }
}

void averageTimePerType() {
  if (hasTickets() == -1)
    return;
  // Projeto académico, para testar nao vamos ate 100 users
  long minutos[100] = {0};
  int contagem[100] = {0};

  ELEM_TICKET *temp = headTickets;

  while (temp != NULL) {
    if ((temp->data.status == STATUS_RESOLVED ||
         temp->data.status == STATUS_CLOSED)) {

      int id = temp->data.typeId;
      long diff = differenceInMinutes(temp->data.openedAt, temp->data.closedAt);

      minutos[id] += diff;
      contagem[id]++;
    }
    temp = temp->next;
  }

  printf("\n--- TEMPO MEDIO DE RESOLUCAO POR TIPO ---\n");
  printf("%-10s | %-15s | %-10s\n", "Tipo", "Tempo Medio", "Tickets");
  printf("-----------+-----------------+------------\n");

  int found = 0;
  for (int i = 0; i < 100; i++) {
    if (contagem[i] > 0) {
      char typeName[50];
      getTypeUtil(i, typeName);

      float media = (float)minutos[i] / contagem[i];
      float horas = media / 60;

      printf("%-10s | %.1f horas       | %d\n", typeName, horas, contagem[i]);
      found = 1;
    }
  }

  if (!found) {
    puts("Nenhum ticket resolvido");
  }
}

int exportTicketsCSV(char *username, int userId) {
  if (hasTickets() == -1)
    return -1;

  char fileName[60];

  snprintf(fileName, sizeof(fileName), "%s_tickets_report.csv", username);

  FILE *fp = fopen(fileName, "w");

  if (fp == NULL) {
    puts("Erro ao criar o ficheiro CSV");
    return -1;
  }

  fprintf(fp, "ID,Tipo,Prioridade,Estado,Utilizador,Data Abertura\n");

  ELEM_TICKET *temp = headTickets;

  char charType[50];
  char charPriority[20];
  char charStatus[20];

  int count = 0;

  while (temp != NULL) {
    if (temp->data.technicianId == userId) {
      getTypeUtil(temp->data.typeId, charType);
      getPriority(temp->data.priority, charPriority);
      getStatus(temp->data.status, charStatus);

      fprintf(fp, "%d,%s,%s,%s,%s,%02d/%02d/%04d %02d:%02d\n", temp->data.id,
              charType, charPriority, charStatus, temp->data.user,
              temp->data.openedAt.day, temp->data.openedAt.month,
              temp->data.openedAt.year, temp->data.openedAt.hour,
              temp->data.openedAt.min);

      count++;
    }

    temp = temp->next;
  }

  fclose(fp);

  if (count == 0) {
    printf("\nNenhum ticket para exportar.\n");
    return -1;
  }

  return 0;
}

// Calcular as estatisticas para o relatorio
ReportStats extractStats(DateTime startDate, DateTime endDate) {
  ReportStats stats = {0};
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL) {
    if (compareDates(temp->data.openedAt, startDate) >= 0 &&
        compareDates(temp->data.openedAt, endDate) <= 0) {
      stats.totalTickets++;

      if (temp->data.status == STATUS_RESOLVED ||
          temp->data.status == STATUS_CLOSED) {
        stats.totalResolved++;

        // Calcula o tempo de res. do ticket
        long resTime =
            differenceInMinutes(temp->data.openedAt, temp->data.closedAt);
        // Calcula o tempo total de resolução
        stats.totalMinutesToResolve += resTime;

        // Calc. número de resoluções tendo em conta o SLA
        int slaLimit = getSLA(temp->data.priority);
        if (slaLimit != -1) // Se não der erro executa
        {
          if (resTime > slaLimit)
            stats.slaViolations++;
          else
            stats.slaMet++;
        }
      } else {
        stats.pendingTickets++;
      }

      switch (temp->data.priority) {
      case 4:
        stats.countCritical++;
        break;
      case 3:
        stats.countHigh++;
        break;
      case 2:
        stats.countMedium++;
        break;
      case 1:
        stats.countLow++;
        break;
      }
    }
    temp = temp->next;
  }

  return stats;
}

int generateMonthReport(int month, int year) {
  char fileName[80];
  snprintf(fileName, sizeof(fileName),
           REPORTS_PATH "Relatorio_Mensal_%02d_%04d.txt", month, year);

  // Define as data de começo e fim para passar á função extractStats
  DateTime startDate = {
      .day = 1, .month = month, .year = year, .hour = 0, .min = 0};
  DateTime endDate = {.day = daysInMonth(month, year),
                      .month = month,
                      .year = year,
                      .hour = 23,
                      .min = 59};

  ReportStats stats = extractStats(startDate, endDate);

  char fileHeader[200];
  snprintf(fileHeader, sizeof(fileHeader),
           "             RELATORIO MENSAL DE HELPDESK               \n         "
           "        Mes: %02d / Ano: %04d                      \n",
           month, year);

  if (createStatsToReports(fileName, fileHeader, stats) == 0) {
    printf("\nRelatório guardado como '%s'.\n", fileName);
    return 0;
  } else {
    return -1;
  }

  return 0;
}

int generateWeeklyReport(DateTime startDate) {
  DateTime endDate = addTimeToDateTime(startDate, 144, 0);
  endDate.hour = 23;
  endDate.min = 59;

  char fileName[80];
  snprintf(fileName, sizeof(fileName),
           REPORTS_PATH "Relatorio_Semana_%02d%02d%04d.txt", startDate.day,
           startDate.month, startDate.year);

  // Chama a function para calc. as stats. visto que repetiria muito codigo
  ReportStats stats = extractStats(startDate, endDate);

  char fileHeader[200];
  snprintf(fileHeader, sizeof(fileHeader),
           "             RELATORIO SEMANAL DE HELPDESK              \n        "
           "Periodo: %02d/%02d/%04d a %02d/%02d/%04d        \n",
           startDate.day, startDate.month, startDate.year, endDate.day,
           endDate.month, endDate.year);

  if (createStatsToReports(fileName, fileHeader, stats) == 0) {
    printf("\nRelatório guardado como '%s'.\n", fileName);
    return 0;
  } else {
    return -1;
  }
}

int createStatsToReports(char *fileName, char *fileHeader, ReportStats stats) {
  FILE *fp = fopen(fileName, "w");
  if (fp == NULL) {
    puts("Erro ao criar ficheiro de relatorio");
    return -1;
  }

  fprintf(fp, "========================================================\n");
  fprintf(fp, "%s", fileHeader);
  fprintf(fp, "========================================================\n\n");

  if (stats.totalTickets == 0) {
    fprintf(fp, "Nenhum ticket reportado neste periodo.\n");
  } else {
    float successRate =
        ((float)stats.totalResolved / stats.totalTickets) * 100.0;
    long avgResolutionTime =
        (stats.totalResolved > 0)
            ? (stats.totalMinutesToResolve / stats.totalResolved)
            : 0;

    fprintf(fp, "[1] VOLUME E PRODUTIVIDADE\n");
    fprintf(fp, "--------------------------------------------------------\n");
    fprintf(fp, "Total de tickets abertos:     %d\n", stats.totalTickets);
    fprintf(fp, "Tickets resolvidos/fechados:  %d (%.1f%%)\n",
            stats.totalResolved, successRate);
    fprintf(fp, "Tickets pendentes:            %d\n\n", stats.pendingTickets);

    fprintf(fp, "[2] DISTRIBUICAO POR PRIORIDADE\n");
    fprintf(fp, "--------------------------------------------------------\n");
    fprintf(fp, "  - Critica: %d\n", stats.countCritical);
    fprintf(fp, "  - Alta:    %d\n", stats.countHigh);
    fprintf(fp, "  - Media:   %d\n", stats.countMedium);
    fprintf(fp, "  - Baixa:   %d\n\n", stats.countLow);

    fprintf(fp, "[3] TEMPOS E SLA\n");
    fprintf(fp, "--------------------------------------------------------\n");
    if (stats.totalResolved > 0) {
      fprintf(fp, "Tempo medio de resolucao:     %ld horas e %ld minutos\n",
              avgResolutionTime / 60, avgResolutionTime % 60);
      fprintf(fp, "SLA Cumprido:                 %d ticket(s)\n", stats.slaMet);
      fprintf(fp, "SLA Violado:                  %d ticket(s)\n",
              stats.slaViolations);
    } else {
      fprintf(fp, "Sem dados de resolucao calculaveis para este periodo.\n");
    }
  }

  fprintf(fp, "\n========================================================\n");
  fprintf(fp, "Relatorio gerado automaticamente pelo Sistema.\n");

  fclose(fp);

  return 0;
}

int alertTicketSLA(int *alertSLA) {

  ELEM_TICKET *temp = headTickets;
  FILE *fp = fopen("alertSLA.dat", "wb");

  *alertSLA = 1;

  if (fp == NULL) {
    puts("Nao foi possivel criar o ficheiro de alertas SLA");
    return -1;
  }

  while (temp != NULL) {

    // Caso ticket ainda aberto, ou seja, nao tem data fim
    if ((temp->data.closedAt.year == 0) &&
        differenceInMinutes(temp->data.openedAt, getCurrentDateTime()) >
            getSLA(temp->data.priority)) {
      fwrite(&(temp->data), sizeof(TICKET_INFO), 1, fp);

      *alertSLA = 0; // Ativar o alerta
    }

    temp = temp->next;
  }

  fclose(fp);

  if (*alertSLA) {
    puts("Nenhum ticket violou o SLA");
    remove("alertSLA.dat");
    return -1;
  }
  return 0;
}

void printAlertsSLA() {
  FILE *fp = fopen("alertSLA.dat", "rb");
  if (fp == NULL) {
    puts("Nao existem alertas SLA de momento ou erro ao abrir ficheiro");
    return;
  }

  TICKET_INFO temp;

  tableHeaders();

  int found = 0;

  while (fread(&temp, sizeof(TICKET_INFO), 1, fp) == 1) {
    printInfoFormatTable(temp);
    found = 1;
  }

  fclose(fp);

  if (!found) {
    puts("\nNenhum ticket encontrado no ficheiro de alertas");
  }
}

int saveTicketsToFile(const char *filename) {
  if (hasTickets() == -1)
    return -1;
  FILE *fp = fopen(filename, "wb");
  if (fp == NULL) {
    printf("\nERRO: Não foi possível guardar tickets\n");
    return -1;
  }

  // Guardo sempre no ficheiro a quantidade de tickets que existem
  int count = getTicketCount();
  fwrite(&count, sizeof(int), 1, fp);

  ELEM_TICKET *temp = headTickets;
  while (temp != NULL) {
    // Guarda ja
    fwrite(&(temp->data), sizeof(TICKET_INFO), 1, fp);

    // AQui so verifico a quantidade de historicos que tem nesse ticket
    int histCount = 0;
    ELEM_HISTORY *hist = temp->history;
    while (hist != NULL) {
      histCount++;
      hist = hist->next;
    }

    // Aqui guardo a quantidade de historicos pertencentes ao ticket
    fwrite(&histCount, sizeof(int), 1, fp);

    // Volta ao historico inicial do ticket
    hist = temp->history;
    while (hist != NULL) {
      fwrite(&(hist->data), sizeof(HISTORY_INFO), 1, fp);
      hist = hist->next;
    }

    temp = temp->next;
  }

  fclose(fp);
  printf("%d tickets guardados em: %s\n", count, filename);
  return 0;
}

int loadTicketsFromFile(const char *filename) {
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("Ficheiro %s não encontrado\n", filename);
    return -1;
  }

  int count;
  if (fread(&count, sizeof(int), 1, fp) != 1) {
    fclose(fp);
    return -1;
  }

  ELEM_TICKET *last = NULL;

  for (int i = 0; i < count; i++) {
    ELEM_TICKET *new = malloc(sizeof(ELEM_TICKET));
    if (new == NULL) {
      fclose(fp);
      return -1;
    }

    // Ler dados do ticket
    if (fread(&(new->data), sizeof(TICKET_INFO), 1, fp) != 1) {
      free(new);
      fclose(fp);
      return -1;
    }

    // Ler histórico
    int histCount;
    if (fread(&histCount, sizeof(int), 1, fp) != 1) {
      fclose(fp);
      return -1;
    }

    new->history = NULL;
    ELEM_HISTORY *lastHist = NULL; // para manter ordem do histórico também

    for (int j = 0; j < histCount; j++) {
      ELEM_HISTORY *newHist = malloc(sizeof(ELEM_HISTORY));
      if (newHist == NULL) {
        fclose(fp);
        return -1;
      }

      if (fread(&(newHist->data), sizeof(HISTORY_INFO), 1, fp) != 1) {
        free(newHist);
        fclose(fp);
        return -1;
      }
      newHist->next = NULL;

      // Inserir no FINAL do histórico (mantém ordem original)
      if (new->history == NULL) {
        new->history = newHist;
        lastHist = newHist;
      } else {
        lastHist->next = newHist;
        lastHist = newHist;
      }
    }

    // Inserir ticket no FINAL da lista principal
    new->next = NULL;
    if (headTickets == NULL) {
      headTickets = new;
      last = new;
    } else {
      last->next = new;
      last = new;
    }
  }

  fclose(fp);
  printf("%d tickets carregados de: %s\n", count, filename);
  return 0;
}

int createPeriodicReports() {
  if (headTickets == NULL) {
    puts("Nenhum ticket registado no sistema para gerar relatorio");
    return -1;
  }

  char fileName[100];
  DateTime currentDate;
  currentDate = getCurrentDateTime();

  snprintf(fileName, sizeof(fileName),
           REPORTS_PATH "Relatorio_Periodico_%02d-%02d-%04d.txt",
           currentDate.day, currentDate.month, currentDate.year);

  FILE *fp = fopen(fileName, "w");
  if (fp == NULL) {
    puts("Erro ao criar ficheiro de relatorio periodico");
    return -1;
  }

  ELEM_TICKET *temp;
  char typeName[50];

  fprintf(fp, "==================================================\n");
  fprintf(fp, "               TICKETS RESOLVIDOS                 \n");
  fprintf(fp, "==================================================\n\n");

  temp = headTickets;
  int countResolved = 0;

  while (temp != NULL) {
    if (temp->data.status == STATUS_RESOLVED ||
        temp->data.status == STATUS_CLOSED) {
      getTypeUtil(temp->data.typeId, typeName);

      fprintf(fp, "ID: #%d | Categoria: %s | Prioridade: %d\n", temp->data.id,
              typeName, temp->data.priority);
      fprintf(fp, "Utilizador: %s | ID Tecnico: %d\n", temp->data.user,
              temp->data.technicianId);
      fprintf(fp, "Descricao: %s\n", temp->data.description);
      fprintf(fp, "Aberto em: %02d/%02d/%04d | Fechado em: %02d/%02d/%04d\n",
              temp->data.openedAt.day, temp->data.openedAt.month,
              temp->data.openedAt.year, temp->data.closedAt.day,
              temp->data.closedAt.month, temp->data.closedAt.year);
      fprintf(fp, "--------------------------------------------------\n");
      countResolved++;
    }
    temp = temp->next;
  }
  if (countResolved == 0) {
    fprintf(fp, "Nenhum ticket resolvido ou fechado ate ao momento.\n");
  }

  fprintf(fp, "\n\n");

  fprintf(fp, "==================================================\n");
  fprintf(fp, "          TICKETS REGISTADOS E ATIVOS             \n");
  fprintf(fp, "==================================================\n\n");

  temp = headTickets;
  int countActive = 0;

  while (temp != NULL) {
    if (temp->data.status != STATUS_RESOLVED &&
        temp->data.status != STATUS_CLOSED) {
      getTypeUtil(temp->data.typeId, typeName);

      char statusText[30];
      getStatus(temp->data.status, statusText);

      fprintf(fp, "ID: #%d | Categoria: %s | Prioridade: %d\n", temp->data.id,
              typeName, temp->data.priority);
      fprintf(fp, "Utilizador: %s | Estado: %s\n", temp->data.user, statusText);
      fprintf(fp, "Descricao: %s\n", temp->data.description);
      fprintf(fp, "Aberto em: %02d/%02d/%04d %02d:%02d\n",
              temp->data.openedAt.day, temp->data.openedAt.month,
              temp->data.openedAt.year, temp->data.openedAt.hour,
              temp->data.openedAt.min);
      fprintf(fp, "--------------------------------------------------\n");
      countActive++;
    }
    temp = temp->next;
  }
  if (countActive == 0) {
    fprintf(fp, "Nenhum ticket ativo ou pendente de momento.\n");
  }

  fprintf(fp, "\n==================================================\n");
  fprintf(fp, "Fim do Relatorio Periodico Geral.\n");

  fclose(fp);

  printf("\nRelatório guardado como '%s'.\n", fileName);
  return 0;
}

void seederTickets() {
  TICKET_INFO t;

  t.typeId = 1;
  t.priority = 1;
  strcpy(t.user, "Maria Joao");
  strcpy(t.description,
         "O rato do computador nao responde nem acende a luz laser");
  createTicket(t);

  t.typeId = 2;
  t.priority = 2;
  strcpy(t.user, "Rui Costa");
  strcpy(t.description, "O Microsoft Excel bloqueia imediatamente sempre que "
                        "tento abrir o ficheiro de contabilidade");
  createTicket(t);

  t.typeId = 3;
  t.priority = 4;
  strcpy(t.user, "Carlos Silva");
  strcpy(t.description, "Todo o departamento de vendas esta completamente sem "
                        "acesso a internet desde as 9h00");
  createTicket(t);

  t.typeId = 4;
  t.priority = 3;
  strcpy(t.user, "Ana Ferreira");
  strcpy(t.description, "Conta bloqueada no sistema ERP apos falhar a password "
                        "3 vezes. Preciso de reset urgente");
  createTicket(t);

  t.typeId = 5;
  t.priority = 1;
  strcpy(t.user, "Sofia Mendes");
  strcpy(t.description, "A cadeira do meu posto de trabalho partiu uma roda e "
                        "esta desequilibrada");
  createTicket(t);

  puts("\n[SISTEMA] 5 Tickets de teste gerados com sucesso!");
}