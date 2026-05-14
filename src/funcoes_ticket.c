#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

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

ELEM_TICKET *headTickets = NULL;

DateTime getCurrentDateTime() {
  DateTime d;
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  d.day = tm->tm_mday;
  d.month = tm->tm_mon + 1;
  d.year = tm->tm_year + 1900;
  d.hour = tm->tm_hour;
  d.min = tm->tm_min;
  return d;
}

// ========================= HISTORY

void addHistory(ELEM_TICKET *ticket, char *user, char *actionType,
                int previousTechnician, int currentTechnician,
                int previousStatus, int currentStatus, char *description) {

  ELEM_HISTORY *newHistory = malloc(sizeof(ELEM_HISTORY));
  if (newHistory == NULL) {
    puts("Erro ao alocar memória para o histórico");
    return;
  }

  newHistory->data.date = getCurrentDateTime();
  strcpy(newHistory->data.user, user);
  strcpy(newHistory->data.actionType, actionType);

  if (previousTechnician == -1)
    strcpy(newHistory->data.previousTechnician, "N/D");
  else
    snprintf(newHistory->data.previousTechnician, 30, "%d", previousTechnician);

  if (currentTechnician == -1)
    strcpy(newHistory->data.currentTechnician, "N/D");
  else
    snprintf(newHistory->data.currentTechnician, 30, "%d", currentTechnician);

  obterEstado(previousStatus, newHistory->data.previousStatus);

  obterEstado(currentStatus, newHistory->data.currentStatus);

  strcpy(newHistory->data.description, description);

  newHistory->next = ticket->history;
  ticket->history = newHistory;
}

int createTicket(TICKET_INFO ticket) {
  ELEM_TICKET *new = malloc(sizeof(ELEM_TICKET));
  if (new == NULL) {
    puts("Erro ao alocar memória");
    return -1;
  }

  new->data = ticket;
  new->data.id = getTicketCount() + 1;
  new->data.technicianId = -1; // deixar -1 porque nao tem tecnico associado
  new->data.status = STATUS_OPEN;
  strcpy(new->data.solution, "");
  new->data.openedAt = getCurrentDateTime();
  new->data.closedAt.day = 0;
  new->data.closedAt.month = 0;
  new->data.closedAt.year = 0;
  new->data.closedAt.hour = 0;
  new->data.closedAt.min = 0;
  new->history = NULL;

  new->next = headTickets;
  headTickets = new;

  printf("\nTicket #%d criado com sucesso!\n", new->data.id);
  return 0;
}

int updateTicket() {
  int idForSearch, option, running = 1;

  printf("\n\nIntroduza o número do TICKET que deseja editar: ");
  scanf("%d", &idForSearch);

  ELEM_TICKET *temp = headTickets;

  while (temp != NULL) {
    if (temp->data.id == idForSearch) {
      puts("\n=== TICKET ENCONTRADO ===");
      printInfosTicket(temp->data);

      do {
        puts("=== ALTERAR INFORMAÇÕES ===");
        puts("1 - Alterar Tipo");
        puts("2 - Alterar Descrição");
        puts("3 - Alterar Prioridade");
        puts("4 - Alterar Estado");
        puts("0 - Voltar");
        printf("Opção: ");
        scanf("%d", &option);
        clearBuffer();
        // AVISO Falta informar o user se a actualização foi bem sucedida (isso
        // será feito quando forem feitas as validações)

        switch (option) {
        case 1:
          // Alterar Tipo
          do {
            printf("Novo tipo (1-Hardware, 2-Software, 3-Rede, 4-Acesso, "
                   "5-Outro): ");

            if (scanf("%d", &temp->data.type) != 1 ||
                (temp->data.type < TYPE_HARDWARE ||
                 temp->data.type > TYPE_OTHER)) // Se o retorno for diferente de
                                                // 1 é porque não leu 1 inteiro.
            {
              puts("Tipo de TICKET não válido.");
              clearBuffer();
            } else {
              break; // Input válido, sai do ciclo
            }
          } while (1);

          clearBuffer();
          break;
        case 2:
          // Alterar Descrição
          printf("Descrição: ");
          fgets(temp->data.description, sizeof(temp->data.description), stdin);
          break;
        case 3:
          // Alterar Prioridade
          printf("Prioridade (1-Baixa, 2-Media, 3-Alta, 4-Critica): ");
          scanf("%d", &temp->data.priority);
          clearBuffer();
          break;
        case 4:
          // Alterar Estado
          printf("Estado (1-Aberto, 2-Em Atend., 3-Esp. User, 4-Resolvido, "
                 "5-Fechado): ");
          scanf("%d", &temp->data.status);
          clearBuffer();

          break;
        case 0:
          running = 0;
          return 0;
        default:
          puts("Opção Inválida!");
          break;
        }
        waitForKey();
      } while (running);

      return 0;
    }

    temp = temp->next;
  }
  printf("\nTicket não encontrado!\n");
  return -1;
}

void printInfosTicket(TICKET_INFO ticket) {
  char tipo[15], status[20], prioridade[10];

  obterTipo(ticket.type, tipo);
  obterEstado(ticket.status, status);
  obterPrioridade(ticket.priority, prioridade);

  printf("========================================\n");
  printf("Ticket #%d\n", ticket.id);
  printf("========================================\n");
  printf("Tipo:        %s\n", tipo);
  printf("Estado:      %s\n", status);
  printf("Prioridade:  %s\n", prioridade);
  printf("Descricao:   %s\n", ticket.description);
  printf("Utilizador:  %s\n", ticket.user);

  printf("Tecnico:     ");
  if (ticket.technicianId == -1)
    printf("N/D\n");
  else
    printf("%d\n", ticket.technicianId);

  printf("Abertura:    %02d/%02d/%04d %02d:%02d\n", ticket.openedAt.day,
         ticket.openedAt.month, ticket.openedAt.year, ticket.openedAt.hour,
         ticket.openedAt.min);

  printf("Solucao:     %s\n",
         strcmp(ticket.solution, "") == 0 ? "N/D" : ticket.solution);
  printf("\n");

  if (ticket.closedAt.day == 0 && ticket.closedAt.month == 0 &&
      ticket.closedAt.year == 0 && ticket.closedAt.hour == 0 &&
      ticket.closedAt.min == 0) {
    printf("Fecho:      N/D\n");
  } else {
    printf("Fecho:      %02d/%02d/%04d %02d:%02d\n", ticket.closedAt.day,
           ticket.closedAt.month, ticket.closedAt.year, ticket.closedAt.hour,
           ticket.closedAt.min);
  }
}

void listAllTickets() {
  if (headTickets == NULL) {
    puts("Nenhum ticket registado.");
    return;
  }

  ELEM_TICKET *temp = headTickets;

  printf("\n%-5s | %-9s | %-12s | %-10s | %-15s | %-10s\n", "ID", "Tipo",
         "Estado", "Prioridade", "Utilizador", "Tecnico");
  printf("------+-----------+--------------+------------+-----------------+----"
         "--------\n");

  while (temp != NULL) {
    char tipo[15], status[20], prioridade[10];

    obterTipo(temp->data.type, tipo);
    obterEstado(temp->data.status, status);
    obterPrioridade(temp->data.priority, prioridade);

    if (temp->data.technicianId == -1) {
      printf("%-5d | %-9s | %-12s | %-10s | %-15s | %-10s\n", temp->data.id,
             tipo, status, prioridade, temp->data.user, "N/D");
    } else {
      printf("%-5d | %-9s | %-12s | %-10s | %-15s | %-10d\n", temp->data.id,
             tipo, status, prioridade, temp->data.user,
             temp->data.technicianId);
    }

    temp = temp->next;
  }
  printf("\n");
}

int getTicketCount() {
  ELEM_TICKET *temp = headTickets;
  int count = 0;
  while (temp != NULL) {
    count++;
    temp = temp->next;
  }
  return count;
}

int existeUserbyId(int id) {
  ELEM_TICKET *temp = headTickets;
  while (temp != NULL) {
    if (temp->data.id == id) {
      return 0;
    }
    temp = temp->next;
  }
  return -1;
}

void showTicketById(int id) {
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL) {
    if (temp->data.id == id) {
      printInfosTicket(temp->data);
      return;
    }
    temp = temp->next;
  }
  puts("Ticket nao encontrado.");
}

int deleteTicket(int id) {
  int confirmDelete;

  if (headTickets == NULL) {
    puts("Nenhum ticket registado para remover.");
    return -1;
  }

  ELEM_TICKET *temp = headTickets;
  ELEM_TICKET *prev = NULL;

  while (temp != NULL && temp->data.id != id) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL) {
    printf("Ticket #%d não encontrado!\n", id);
    return -1;
  }

  printInfosTicket(temp->data);

  // Confirmação da remoção
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

  clearBuffer();

  if (confirmDelete == 1) {
    puts("Remoção cancelada");
    return -1;
  }

  // remover da lista
  if (prev == NULL) {
    headTickets = temp->next;
  } else {
    prev->next = temp->next;
  }

  // libertar histórico
  ELEM_HISTORY *histTemp = temp->history;

  while (histTemp != NULL) {
    ELEM_HISTORY *aux = histTemp;
    histTemp = histTemp->next;
    free(aux);
  }

  free(temp);

  printf("Ticket #%d removido com sucesso!\n", id);
  return 0;
}

int assignTechnician(int ticket_id, int technicianId) {
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL) {
    if (temp->data.id == ticket_id) {
      if (isTechnicianValidated(technicianId) == 1) {
        puts("Técnico está valido para ser associado");
      } else if (isTechnicianValidated(technicianId) == 0) {
        puts("Técnico deve ser validado");
        return -1;
      } else {
        puts("Técnico não encontrado");
        return -1;
      }

      int tecnicoAnterior = temp->data.technicianId;
      int estadoAnterior = temp->data.status;

      temp->data.technicianId = technicianId;

      if (temp->data.status == STATUS_OPEN) {
        temp->data.status = STATUS_IN_PROGRESS;
      }

      char descricao[300];
      // snprintf permite passar argumentos e guarda na variavel descrição
      snprintf(descricao, 300, "Ticket atribuido ao tecnico #%d", technicianId);

      addHistory(temp, "admin", "ATRIBUICAO", tecnicoAnterior, technicianId,
                 estadoAnterior, temp->data.status, descricao);

      printf("Ticket #%d atribuido ao tecnico #%d com sucesso!\n", ticket_id,
             technicianId);
      return 0;
    }
    temp = temp->next;
  }

  puts("Ticket nao encontrado!");
  return -1;
}