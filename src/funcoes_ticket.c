#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define REPORTS_PATH "reports\\"

ELEM_TICKET *headTickets = NULL;

DateTime getCurrentDateTime()
{
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
void printTicketHistory(int ticketId) { printHistory(ticketId, headTickets); }
void deleteType(int typeId) { deleteTicketType(typeId, headTickets); }

int nextTicketId = 1;

void seederTickets()
{
    TICKET_INFO t;

    t.typeId = 1;
    t.priority = 1; 
    strcpy(t.user, "Maria Joao");
    strcpy(t.description, "O rato do computador nao responde nem acende a luz laser.");
    createTicket(t);

    t.typeId = 2;
    t.priority = 2; 
    strcpy(t.user, "Rui Costa");
    strcpy(t.description, "O Microsoft Excel bloqueia imediatamente sempre que tento abrir o ficheiro de contabilidade.");
    createTicket(t);

    t.typeId = 3;
    t.priority = 4; 
    strcpy(t.user, "Carlos Silva");
    strcpy(t.description, "Todo o departamento de vendas esta completamente sem acesso a internet desde as 9h00.");
    createTicket(t);

    t.typeId = 4; 
    t.priority = 3; 
    strcpy(t.user, "Ana Ferreira");
    strcpy(t.description, "Conta bloqueada no sistema ERP apos falhar a password 3 vezes. Preciso de reset urgente.");
    createTicket(t);

    t.typeId = 5; 
    t.priority = 1; 
    strcpy(t.user, "Sofia Mendes");
    strcpy(t.description, "A cadeira do meu posto de trabalho partiu uma roda e esta desequilibrada.");
    createTicket(t);

    puts("\n[SISTEMA] 5 Tickets de teste gerados com sucesso!");
}

int createTicket(TICKET_INFO ticket)
{
  ELEM_TICKET *new = malloc(sizeof(ELEM_TICKET));
  if (new == NULL)
  {
    puts("Erro ao alocar memória");
    return -1;
  }

  new->data = ticket;
  new->data.id = nextTicketId;
  nextTicketId++;
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

int deleteTicket(int id)
{
  int isConfirmed;

  if (headTickets == NULL)
  {
    puts("Nenhum ticket registado para remover.");
    return -1;
  }

  ELEM_TICKET *temp = headTickets;
  ELEM_TICKET *prev = NULL;

  while (temp != NULL && temp->data.id != id)
  {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL)
  {
    printf("Ticket #%d não encontrado!\n", id);
    return -1;
  }

  printInfosTicket(temp->data);

  // Confirmação da remoção

  isConfirmed = confirmDelete();

  if (isConfirmed == 1)
  {
    puts("Remoção cancelada");
    return -1;
  }

  // remover da lista
  if (prev == NULL)
  {
    headTickets = temp->next;
  }
  else
  {
    prev->next = temp->next;
  }

  // libertar histórico
  ELEM_HISTORY *histTemp = temp->history;

  while (histTemp != NULL)
  {
    ELEM_HISTORY *aux = histTemp;
    histTemp = histTemp->next;
    free(aux);
  }

  free(temp);

  printf("Ticket #%d removido com sucesso!\n", id);
  return 0;
}

int updateTicket(int ticketId)
{
  int option, running = 1;

  ELEM_TICKET *temp = headTickets;

  while (temp != NULL)
  {
    if (temp->data.id == ticketId)
    {
      puts("\n=== TICKET ENCONTRADO ===");
      printInfosTicket(temp->data);

      do
      {
        puts("=== ALTERAR INFORMAÇÕES ===");
        puts("1 - Alterar Tipo");
        puts("2 - Alterar Descrição");
        puts("3 - Alterar Prioridade");
        puts("4 - Alterar Estado");
        puts("0 - Voltar");
        printf("Opção: ");
        scanf("%d", &option);
        clearBuffer();
        // AVISO Falta informar o user se a actualização foi bem sucedida (isso será feito quando forem feitas as validações)

        switch (option)
        {
        case 1:
          // Alterar Tipo
          int realId, displayId;
          do
          {
            printf("Novo tipo:");
            listTicketTypes();
            printf("Opção: ");
            scanf("%d", &displayId);

            realId = getRealTypeId(displayId);
            if (realId == -1)
            {
              printf("Opção inválida");
            }
            else
            {
              temp->data.typeId = realId;
              break;
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
          printf("Estado (1-Aberto, 2-Em Atend., 3-Esp. User, 4-Resolvido, 5-Fechado): ");
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

void printInfoFormatTable(TICKET_INFO ticket)
{
  char typeStr[9], statusStr[20], priorityStr[10], techStr[20], dataStr[20];

  getTypeUtil(ticket.typeId, typeStr);
  getStatus(ticket.status, statusStr);
  getPriority(ticket.priority, priorityStr);

  if (ticket.technicianId == -1)
  {
    strcpy(techStr, "N/D");
  }
  else
  {
    sprintf(techStr, "%d", ticket.technicianId);
  }

  sprintf(dataStr, "%02d/%02d/%04d %02d:%02d", ticket.openedAt.day,
          ticket.openedAt.month, ticket.openedAt.year, ticket.openedAt.hour,
          ticket.openedAt.min);

  printf("%-5d | %-9s | %-12s | %-10s | %-15s | %-10s | %-16s\n", ticket.id,
         typeStr, statusStr, priorityStr, ticket.user, techStr, dataStr);
}

void printInfosTicket(TICKET_INFO ticket)
{
  char typeStr[15], statusStr[20], priorityStr[10];
  char techStr[20], openDateStr[30], closeDateStr[30];

  getTypeUtil(ticket.typeId, typeStr);
  getStatus(ticket.status, statusStr);
  getPriority(ticket.priority, priorityStr);

  // Formatar string do Técnico
  if (ticket.technicianId == -1)
  {
    strcpy(techStr, "N/D");
  }
  else
  {
    sprintf(techStr, "%d", ticket.technicianId);
  }

  // Formatar string de Abertura
  sprintf(openDateStr, "%02d/%02d/%04d %02d:%02d", ticket.openedAt.day,
          ticket.openedAt.month, ticket.openedAt.year, ticket.openedAt.hour,
          ticket.openedAt.min);

  if (ticket.closedAt.day == 0 && ticket.closedAt.month == 0 &&
      ticket.closedAt.year == 0)
  {
    strcpy(closeDateStr, "N/D");
  }
  else
  {
    sprintf(closeDateStr, "%02d/%02d/%04d %02d:%02d", ticket.closedAt.day,
            ticket.closedAt.month, ticket.closedAt.year, ticket.closedAt.hour,
            ticket.closedAt.min);
  }

  printf("===============================================================\n");
  printf(" TICKET #%d\n", ticket.id);
  printf("===============================================================\n");

  printf("%-12s %-18s | %-12s %-18s\n", "Tipo:", typeStr, "Estado:", statusStr);
  printf("%-12s %-18s | %-12s %-18s\n", "Prioridade:", priorityStr,
         "Utilizador:", ticket.user);
  printf("%-12s %-18s | %-12s %-18s\n", "Tecnico:", techStr,
         "Abertura:", openDateStr);
  printf("%-12s %-18s | %-12s %-18s\n", "", "", "Fecho:", closeDateStr);

  printf("---------------------------------------------------------------\n");
  printf("Descricao:   %s\n", ticket.description);
  printf("Solucao:     %s\n",
         (strcmp(ticket.solution, "") == 0) ? "N/D" : ticket.solution);
  printf("===============================================================\n\n");
}

void listAllTickets()
{
  if (headTickets == NULL)
  {
    puts("Nenhum ticket registado.");
    return;
  }

  ELEM_TICKET *temp = headTickets;
  tableHeaders();
  while (temp != NULL)
  {
    printInfoFormatTable(temp->data);

    temp = temp->next;
  }
  printf("\n");
}

int getTicketCount()
{
  ELEM_TICKET *temp = headTickets;
  int count = 0;
  while (temp != NULL)
  {
    count++;
    temp = temp->next;
  }
  return count;
}

int existeUserbyId(int id)
{
  ELEM_TICKET *temp = headTickets;
  while (temp != NULL)
  {
    if (temp->data.id == id)
    {
      return 0;
    }
    temp = temp->next;
  }
  return -1;
}

void showTicketById(int id)
{
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL)
  {
    if (temp->data.id == id)
    {
      printInfosTicket(temp->data);
      return;
    }
    temp = temp->next;
  }
  puts("Ticket nao encontrado.");
}

void showTicketByTechnician(int id)
{
  if (headTickets == NULL)
  {
    puts("Nenhum ticket registado no sistema.");
    return;
  }

  ELEM_TICKET *temp = headTickets;

  tableHeaders();

  while (temp != NULL)
  {
    if (temp->data.technicianId == id)
    {
      printInfoFormatTable(temp->data);
    }
    temp = temp->next;
  }
  printf("\n");
}

int assignTechnician(int ticket_id, int technicianId)
{
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL)
  {
    if (temp->data.id == ticket_id)
    {
      if (isTechnicianValidated(technicianId) == -1)
      {
        puts("Tecnico nao encontrado");
        return -1;
      }
      if (isTechnicianValidated(technicianId) == 0)
      {
        puts("Tecnico deve ser validado");
        return -1;
      }
      HISTORY_INFO h;

      h.date = getCurrentDateTime();
      strcpy(h.user, "admin");
      strcpy(h.actionType, "ATRIBUICAO");
      if (temp->data.technicianId == -1)
      {
        strcpy(h.previousTechnician, "N/D");
      }
      else
      {
        snprintf(h.previousTechnician, 30, "%d", temp->data.technicianId);
      }
      snprintf(h.currentTechnician, 30, "%d", technicianId);
      getStatus(temp->data.status, h.previousStatus);
      temp->data.technicianId = technicianId;

      if (temp->data.status == STATUS_OPEN)
      {
        temp->data.status = STATUS_IN_PROGRESS;
      }
      getStatus(temp->data.status, h.currentStatus);
      snprintf(h.description, 300, "Ticket atribuido ao tecnico #%d",
               technicianId);

      // Adicionar histórico
      addHistory(temp, h);

      printf("Ticket #%d atribuido ao tecnico #%d com sucesso!\n", ticket_id,
             technicianId);
      return 0;
    }
    temp = temp->next;
  }

  puts("Ticket nao encontrado!");
  return -1;
}

// ========================== Listar Tickets por FILTROS =================================

void listTicketsByStatus(int status)
{
  if (headTickets == NULL)
  {
    puts("Nenhum ticket registado no sistema.");
    return;
  }

  ELEM_TICKET *temp = headTickets;
  int found = 0;

  tableHeaders();

  while (temp != NULL)
  {
    if (temp->data.status == status)
    {
      printInfoFormatTable(temp->data);
      found = 1;
    }
    temp = temp->next;
  }

  if (found == 0)
  {
    printf("Nenhum ticket encontrado com esse estado.\n");
  }
  printf("\n");
}

void listTicketsByPriority(int priority)
{
  if (headTickets == NULL)
  {
    puts("Nenhum ticket registado no sistema.");
    return;
  }

  ELEM_TICKET *temp = headTickets;
  int found = 0;

  tableHeaders();

  while (temp != NULL)
  {
    if (temp->data.priority == priority)
    {
      printInfoFormatTable(temp->data);
      found = 1;
    }
    temp = temp->next;
  }

  if (found == 0)
  {
    printf("Nenhum ticket encontrado com essa prioridade.\n");
  }
  printf("\n");
}

void listTicketsByType(int type)
{
  if (headTickets == NULL)
  {
    puts("Nenhum ticket registado no sistema.");
    return;
  }

  ELEM_TICKET *temp = headTickets;
  int found = 0;

  tableHeaders();

  while (temp != NULL)
  {
    if (temp->data.typeId == type)
    {

      printInfoFormatTable(temp->data);
      found = 1;
    }
    temp = temp->next;
  }

  if (found == 0)
  {
    printf("Nenhum ticket encontrado com esse tipo.\n");
  }
  printf("\n");
}

// ========================== Ordenar a lista por tipo =================================

void sortTicketsByPriority()
{
  if (headTickets == NULL || headTickets->next == NULL)
  {
    puts("Operação impossivel de executar");
    return;
  }

  ELEM_TICKET *limit = NULL;
  int swapped = 1;

  while (swapped)
  {
    swapped = 0;
    ELEM_TICKET *current = headTickets;

    while (current->next != limit)
    {
      if (current->data.priority < current->next->data.priority)
      {
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

void sortTicketsByDate()
{
  if (headTickets == NULL || headTickets->next == NULL)
  {
    puts("Operação impossivel de executar");
    return;
  }

  ELEM_TICKET *limit = NULL;
  int swapped = 1;

  while (swapped)
  {
    swapped = 0;
    ELEM_TICKET *current = headTickets;

    while (current->next != limit)
    {
      if (compareDates(current->data.openedAt, current->next->data.openedAt) >
          0)
      {
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

void sortTicketsByTechnician()
{
  if (headTickets == NULL || headTickets->next == NULL)
  {
    puts("Operação impossivel de executar");
    return;
  }

  ELEM_TICKET *limit = NULL;
  int swapped = 1;

  while (swapped)
  {
    swapped = 0;
    ELEM_TICKET *current = headTickets;

    while (current->next != limit)
    {
      if (current->data.technicianId > current->next->data.technicianId)
      {
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

int updateTicketStatus(int ticket_id, int logged_userId)
{
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL && temp->data.id != ticket_id)
  {
    temp = temp->next;
  }

  if (temp == NULL)
  {
    printf("Ticket #%d não encontrado.\n", ticket_id);
    return -1;
  }

  if (temp->data.technicianId != logged_userId && isAdmin(logged_userId) == -1)
  {
    printf("Apenas é possível alterar o estado dos tickets a si atribuidos");
    return -1;
  }

  char strStatus[30];
  getStatus(temp->data.status, strStatus);

  printf("Estado atual: %d - %s", temp->data.status, strStatus);

  int newStatus;
  int isValid = 0;

  do
  {
    printf("\nIntroduza o novo estado (1-Aberto, 2-Em Atendimento, 3-Esp. User, 4-Resolvido): ");

    if (scanf("%d", &newStatus) == 1)
    {
      clearBuffer();

      if (newStatus == STATUS_OPEN && (temp->data.status == STATUS_IN_PROGRESS || temp->data.status == STATUS_WAITING_USER))
      {
        puts("Erro: Não é possível reverter um ticket em Atendimento/Espera para Aberto.");
      }
      else if (newStatus >= STATUS_OPEN && newStatus <= STATUS_CLOSED)
      {
        isValid = 1;
      }
      else
      {
        puts("Estado inválido.");
      }
    }
    else
    {
      clearBuffer();
      puts("Erro: Entrada inválida. Introduza um número.");
    }

  } while (!isValid);

  if (newStatus == STATUS_RESOLVED)
  {
    printf("Introduza uma solução para o ticket ser fechado: ");
    fgets(temp->data.solution, sizeof(temp->data.solution), stdin);
    temp->data.solution[strcspn(temp->data.solution, "\n")] = 0;
    temp->data.closedAt = getCurrentDateTime();
  }

  getStatus(newStatus, strStatus);
  temp->data.status = newStatus;

  printf("Estado do ticket #%d alterado para '%s' com sucesso!\n", ticket_id, strStatus);
  return 0;
}

int generateMonthReport(int month, int year)
{

  char fileName[50];

  sprintf(fileName, REPORTS_PATH "Relatorio_%d_%d.txt", month, year);

  FILE *fp = fopen(fileName, "w");
  if (fp == NULL)
  {
    printf("Erro ao criar ficheiro.");
    return -1;
  }

  int totalTickets = 0;

  ELEM_TICKET *temp = headTickets;

  while (temp != NULL)
  {
    if (temp->data.openedAt.month == month && temp->data.openedAt.year == year)
    {
      totalTickets++;
    }
    temp = temp->next;
  }

  fprintf(fp, "========================================================\n");
  fprintf(fp, "             RELATORIO MENSAL DE HELPDESK               \n");
  fprintf(fp, "                  %02d/%04d                     \n", month, year);
  fprintf(fp, "========================================================\n\n");

  fprintf(fp, "RESUMO:\n");
  fprintf(fp, "--------------------------------------------------------\n");
  fprintf(fp, "Total de tickets abertos no mes: %d\n", totalTickets);

  fclose(fp);

  return 0;
}
