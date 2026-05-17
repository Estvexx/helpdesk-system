#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <winscard.h>

#define REPORTS_PATH "reports\\"

ELEM_TICKET *headTickets = NULL;

void cleanupIntermediateTickets() { cleanupTickets(headTickets); }

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
  strcpy(t.description,
         "O rato do computador nao responde nem acende a luz laser.");
  createTicket(t);

  t.typeId = 2;
  t.priority = 2;
  strcpy(t.user, "Rui Costa");
  strcpy(t.description, "O Microsoft Excel bloqueia imediatamente sempre que "
                        "tento abrir o ficheiro de contabilidade.");
  createTicket(t);

  t.typeId = 3;
  t.priority = 4;
  strcpy(t.user, "Carlos Silva");
  strcpy(t.description, "Todo o departamento de vendas esta completamente sem "
                        "acesso a internet desde as 9h00.");
  createTicket(t);

  t.typeId = 4;
  t.priority = 3;
  strcpy(t.user, "Ana Ferreira");
  strcpy(t.description, "Conta bloqueada no sistema ERP apos falhar a password "
                        "3 vezes. Preciso de reset urgente.");
  createTicket(t);

  t.typeId = 5;
  t.priority = 1;
  strcpy(t.user, "Sofia Mendes");
  strcpy(t.description, "A cadeira do meu posto de trabalho partiu uma roda e "
                        "esta desequilibrada.");
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
  new->data.openedAt = getCurrentDateTime();
  new->data.closedAt.day = 0;
  new->data.closedAt.month = 0;
  new->data.closedAt.year = 0;
  new->data.closedAt.hour = 0;
  new->data.closedAt.min = 0;
  strcpy(new->data.actions, "");
  strcpy(new->data.tools, "");
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
        // AVISO Falta informar o user se a actualização foi bem sucedida (isso
        // será feito quando forem feitas as validações)

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
  char techStr[20], openDateStr[30], closeDateStr[30],
      estimatedConclusionDateStr[30];

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

  if (ticket.estimatedConclusion.day == 0 &&
      ticket.estimatedConclusion.month == 0 &&
      ticket.estimatedConclusion.year == 0)
  {
    strcpy(estimatedConclusionDateStr, "N/D");
  }
  else
  {
    sprintf(estimatedConclusionDateStr, "%02d/%02d/%04d %02d:%02d",
            ticket.closedAt.day, ticket.closedAt.month, ticket.closedAt.year,
            ticket.closedAt.hour, ticket.closedAt.min);
  }

  printf("===============================================================\n");
  printf(" TICKET #%d\n", ticket.id);
  printf("===============================================================\n");

  printf("%-12s %-18s | %-12s %-18s\n", "Tipo:", typeStr,
         "Utilizador:", ticket.user);
  printf("%-12s %-18s | %-12s %-18s\n", "Prioridade:", priorityStr,
         "Abertura:", openDateStr);
  printf("%-12s %-18s | %-12s %-18s\n", "Tecnico:", techStr,
         "Previsao:", estimatedConclusionDateStr);
  printf("%-12s %-18s | %-12s %-18s\n", "Estado:", statusStr,
         "Fecho:", closeDateStr);

  printf("---------------------------------------------------------------\n");
  printf("Descricao:   %s\n", ticket.description);
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

  int encontrou = 0;

  ELEM_TICKET *temp = headTickets;

  tableHeaders();

  while (temp != NULL)
  {
    if (temp->data.technicianId == id)
    {
      printInfoFormatTable(temp->data);
      encontrou = 1;
    }
    temp = temp->next;
  }
  if (!encontrou)
  {
    puts("Nenhum atribuido a si");
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

      // Se o status nao for open nao pode atribuir tenico
      if (temp->data.status != STATUS_OPEN)
      {
        puts("Ticket ja esta em atendimento ou fechado. Impossivel alterar "
             "tecnico.");
        return -1;
      }
      HISTORY_INFO h;

      h.date = getCurrentDateTime();
      strcpy(h.user, "admin");
      strcpy(h.actionType, "ATRIBUICAO DE TICKET A TECNICO");
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
        temp->data.status = STATUS_WAITING_USER;
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

// ========================== Listar Tickets por FILTROS
// =================================

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

// ========================== Ordenar a lista por tipo
// =================================

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

  if (temp->data.technicianId != logged_userId &&
      isAdmin(logged_userId) == -1)
  {
    printf("Apenas é possível alterar o estado dos tickets a si atribuidos.\n");
    return -1;
  }

  char strStatus[30];
  getStatus(temp->data.status, strStatus);
  printf("Estado atual: %d - %s\n", temp->data.status, strStatus);

  // Verificar se pode ser alterado
  if (temp->data.status == STATUS_OPEN)
  {
    puts("Ticket em estado Aberto. Utilize 'Aceitar Ticket' primeiro.");
    return -1;
  }
  if (temp->data.status == STATUS_CLOSED)
  {
    puts("Ticket já está fechado. Não pode ser alterado.");
    return -1;
  }

  int newStatus;
  char acao[20];

  if (temp->data.status == STATUS_IN_PROGRESS)
  {
    newStatus = STATUS_RESOLVED;
    strcpy(acao, "RESOLUCAO");
  }
  else if (temp->data.status == STATUS_RESOLVED)
  {
    newStatus = STATUS_CLOSED;
    strcpy(acao, "FECHO");
  }
  else
  {
    printf("Estado '%s' não permite alteração.\n", strStatus);
    return -1;
  }

  char novoStr[30];
  getStatus(newStatus, novoStr);
  printf("Alterar estado para '%s'? (1-Sim / 0-Não): ", novoStr);
  int confirm;
  scanf("%d", &confirm);
  clearBuffer();

  if (confirm != 1)
  {
    puts("Operação cancelada.");
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

  if (newStatus == STATUS_RESOLVED)
  {
    char solucao[500];
    puts("Descreva a solução aplicada:");
    fgets(solucao, sizeof(solucao), stdin);
    solucao[strcspn(solucao, "\n")] = 0;
    temp->data.closedAt = getCurrentDateTime();
    snprintf(h.description, 600, "Ticket resolvido. Solução: %s", solucao);
  }
  else if (newStatus == STATUS_CLOSED)
  {
    puts("Descreva as ações realizadas:\n");
    fgets(temp->data.actions, sizeof(temp->data.actions), stdin);
    temp->data.actions[strcspn(temp->data.actions, "\n")] = 0;

    puts("Ferramentas utilizadas:");
    fgets(temp->data.tools, sizeof(temp->data.tools), stdin);
    temp->data.tools[strcspn(temp->data.tools, "\n")] = 0;

    snprintf(h.description, 800, "Ticket fechado. Ações: %s | Ferramentas: %s",
             temp->data.actions, temp->data.tools);
  }

  temp->data.status = newStatus;

  addHistory(temp, h);

  printf("Estado do ticket #%d alterado para '%s' com sucesso!\n", ticket_id,
         novoStr);

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
    if (temp->data.openedAt.month == month &&
        temp->data.openedAt.year == year)
    {
      totalTickets++;
    }
    temp = temp->next;
  }

  fprintf(fp, "========================================================\n");
  fprintf(fp, "             RELATORIO MENSAL DE HELPDESK               \n");
  fprintf(fp, "                  %02d/%04d                     \n", month,
          year);
  fprintf(fp, "========================================================\n\n");

  fprintf(fp, "RESUMO:\n");
  fprintf(fp, "--------------------------------------------------------\n");
  fprintf(fp, "Total de tickets abertos no mes: %d\n", totalTickets);

  fclose(fp);

  return 0;
}

int acceptTicket(int tecnicoId, int ticketId)
{
  ELEM_TICKET *temp = headTickets;

  while (temp != NULL)
  {
    // É necessario verificar se o ticket está associado a esse mesmo tecnico
    if (temp->data.id == ticketId)
    {
      if (temp->data.technicianId != tecnicoId)
      {
        puts("Este ticket nao esta atribuido a si.");
        return -1;
      }

      // Verificar se o ticket ainda está a espera de repstoa
      if (temp->data.status != STATUS_WAITING_USER)
      {
        puts("Este ticket ja foi aceite ou ja nao esta atribuido a si.");
        return -1;
      }

      int horas, minutos;
      puts("Tempo estimado de resolucao (horas e minutos):");
      // Nestes aqui deixar o printf visto que o utilizador escrever a frente do
      // output
      printf("Horas: ");
      scanf("%d", &horas);
      clearBuffer();
      printf("Minutos: ");
      scanf("%d", &minutos);
      clearBuffer();

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
      getStatus(STATUS_OPEN, h.previousStatus);
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

      printf("\nTicket #%d aceite com sucesso!\n", ticketId);
      return 0;
    }
    temp = temp->next;
  }

  puts("Ticket nao encontrado.");
  return -1;
}

int delegateTicket(int ticket_id, int logged_userId)
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

  if (temp->data.technicianId != logged_userId)
  {
    printf("Este ticket não está atribuído a si.\n");
    return -1;
  }

  if (temp->data.status == STATUS_CLOSED)
  {
    printf("Não é possível delegar um ticket fechado.\n");
    return -1;
  }

  int newTechnicianId;
  listAllTechnicians();
  printf("ID do técnico para delegar: ");
  scanf("%d", &newTechnicianId);
  clearBuffer();

  if (newTechnicianId == logged_userId)
  {
    printf("Não pode delegar um ticket para si próprio.\n");
    return -1;
  }

  if (isTechnicianValidated(newTechnicianId) == -1)
  {
    printf("Técnico #%d não encontrado.\n", newTechnicianId);
    return -1;
  }
  if (isTechnicianValidated(newTechnicianId) == 0)
  {
    printf("Técnico #%d ainda não foi validado.\n", newTechnicianId);
    return -1;
  }

  char motivo[300];
  printf("Motivo da delegação: ");
  fgets(motivo, sizeof(motivo), stdin);
  motivo[strcspn(motivo, "\n")] = 0;

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

  printf("Ticket #%d delegado ao técnico #%d com sucesso!\n", ticket_id,
         newTechnicianId);

  return 0;
}

int addComment(int ticket_id, int logged_userId)
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

  if (temp->data.technicianId != logged_userId)
  {
    puts("Este ticket não está atribuído a si");
    return -1;
  }

  if (temp->data.status == STATUS_CLOSED)
  {
    puts("Não é possível comentar um ticket fechado");
    return -1;
  }

  char comentario[300];
  puts("Comentário: ");
  fgets(comentario, sizeof(comentario), stdin);
  comentario[strcspn(comentario, "\n")] = 0;

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

  printf("Comentário adicionado ao ticket #%d com sucesso!\n", ticket_id);
  return 0;
}

void averageTimePerTechnician()
{
  if (headTickets == NULL)
  {
    puts("Nenhum ticket registado.");
    return;
  }

  long minutos[100] = {0};
  int contagem[100] = {0};

  ELEM_TICKET *temp = headTickets;

  while (temp != NULL)
  {
    if ((temp->data.status == STATUS_RESOLVED ||
         temp->data.status == STATUS_CLOSED) &&
        temp->data.technicianId != -1)
    {

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

  int encontrou = 0;
  for (int i = 0; i < 100; i++)
  {
    if (contagem[i] > 0)
    {
      float media = (float)minutos[i] / contagem[i];
      float horas = media / 60;
      printf("#%-9d | %.1f horas       | %d\n", i, horas, contagem[i]);
      encontrou = 1;
    }
  }

  if (!encontrou)
  {
    puts("Nenhum ticket resolvido.");
  }
}

int exportTicketsCSV(char *username, int userId)
{
  if (headTickets == NULL)
  {
    puts("Não existem tickets registados no sistema.");
    return -1;
  }

  char fileName[60];

  snprintf(fileName, sizeof(fileName), "%s_tickets_report.csv", username);

  FILE *fp = fopen(fileName, "w");

  if (fp == NULL)
  {
    puts("Erro ao criar o ficheiro CSV.");
    return -1;
  }

  fprintf(fp, "ID,Tipo,Prioridade,Estado,Utilizador,Data Abertura\n");

  ELEM_TICKET *temp = headTickets;

  char charType[50];
  char charPriority[20];
  char charStatus[20];
  
  int count = 0; 

  while (temp != NULL)
  {
    if (temp->data.technicianId == userId)
    {
      getTypeUtil(temp->data.typeId, charType); 
      getPriority(temp->data.priority, charPriority);
      getStatus(temp->data.status, charStatus);

      fprintf(fp, "%d,%s,%s,%s,%s,%02d/%02d/%04d %02d:%02d\n", temp->data.id, charType, charPriority, charStatus, temp->data.user,temp->data.openedAt.day, temp->data.openedAt.month, temp->data.openedAt.year,temp->data.openedAt.hour, temp->data.openedAt.min);
      
      count++;
    }
    
    temp = temp->next;
  }

  fclose(fp);

  if (count == 0) 
  {
    printf("Não existem tickets atribuidos a %s para exportar.\n", username);
  } 
  else 
  {
    printf("\nExportação concluída! %d tickets guardados no ficheiro '%s'.\n", count, fileName);
  }

  return 0;
}