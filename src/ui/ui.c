#include "ui.h"
#include "../funcoes.h"
#include <stdio.h>
#include <string.h>

void Navbar() {
  //  printf("\033[0;32m");

  // clang-format off
  /* prettier-ignore */
    printf(
        "    ░██     ░██            ░██                   ░██                       ░██                                                   ░██                               \n"
        "    ░██     ░██            ░██                   ░██                       ░██                                                   ░██                               \n"
        "    ░██     ░██  ░███████  ░██ ░████████   ░████████  ░███████   ░███████  ░██    ░██         ░███████  ░██    ░██  ░███████  ░████████  ░███████  ░█████████████  \n"
        "    ░██████████ ░██    ░██ ░██ ░██    ░██ ░██    ░██ ░██    ░██ ░██        ░██   ░██ ░██████ ░██        ░██    ░██ ░██           ░██    ░██    ░██ ░██   ░██   ░██ \n"
        "    ░██     ░██ ░█████████ ░██ ░██    ░██ ░██    ░██ ░█████████  ░███████  ░███████           ░███████  ░██    ░██  ░███████     ░██    ░█████████ ░██   ░██   ░██ \n"
        "    ░██     ░██ ░██        ░██ ░███   ░██ ░██   ░███ ░██               ░██ ░██   ░██                ░██ ░██   ░███        ░██    ░██    ░██        ░██   ░██   ░██ \n"
        "    ░██     ░██  ░███████  ░██ ░██░█████   ░█████░██  ░███████   ░███████  ░██    ░██         ░███████   ░█████░██  ░███████      ░████  ░███████  ░██   ░██   ░██ \n"
        "                               ░██                                                                             ░██                                                 \n"
        "                               ░██                                                                       ░███████                                                  \n"
    );
  // clang-format on
  // Reset cor
  // printf("\033[0m");
}

void Title(const char *title, const char *icon) {
  int box_width = 50;        // largura interna da caixa
  int icon_visual_width = 2; // emojis ocupam 2 colunas visuais
  int title_len = strlen(title);
  int total_visual =
      icon_visual_width + 1 + title_len; // ícone + espaço + título

  int padding_total = box_width - total_visual;
  int padding_left = padding_total / 2;
  int padding_right = padding_total - padding_left;

  printf("\n");
  printf("╔══════════════════════════════════════════════════╗\n");
  printf("║%*s%s %s%*s║\n", padding_left, "", icon, title, padding_right, "");
  printf("╚══════════════════════════════════════════════════╝\n");
  printf("\n");
}

void printInfoFormatTable(TICKET_INFO ticket) {
  char typeStr[50], statusStr[20], priorityStr[10], techStr[20], dataStr[20];

  getTypeUtil(ticket.typeId, typeStr);
  getStatus(ticket.status, statusStr);
  getPriority(ticket.priority, priorityStr);

  if (ticket.technicianId == -1) {
    strcpy(techStr, "N/D");
  } else {
    sprintf(techStr, "%d", ticket.technicianId);
  }

  sprintf(dataStr, "%02d/%02d/%04d %02d:%02d", ticket.openedAt.day,
          ticket.openedAt.month, ticket.openedAt.year, ticket.openedAt.hour,
          ticket.openedAt.min);

  printf("%-5d | %-18s | %-20s | %-10s | %-18s | %-7s | %-16s | %-5d\n",
         ticket.id, typeStr, statusStr, priorityStr, ticket.user, techStr,
         dataStr, getSLA(ticket.priority));
}

void printInfosTicket(TICKET_INFO ticket) {
  char typeStr[15], statusStr[20], priorityStr[10];
  char techStr[20], openDateStr[30], closeDateStr[30],
      estimatedConclusionDateStr[30];

  getTypeUtil(ticket.typeId, typeStr);
  getStatus(ticket.status, statusStr);
  getPriority(ticket.priority, priorityStr);

  // Formatar string do Técnico
  if (ticket.technicianId == -1) {
    strcpy(techStr, "N/D");
  } else {
    sprintf(techStr, "%d", ticket.technicianId);
  }

  // Formatar string de Abertura
  sprintf(openDateStr, "%02d/%02d/%04d %02d:%02d", ticket.openedAt.day,
          ticket.openedAt.month, ticket.openedAt.year, ticket.openedAt.hour,
          ticket.openedAt.min);

  if (ticket.closedAt.day == 0 && ticket.closedAt.month == 0 &&
      ticket.closedAt.year == 0) {
    strcpy(closeDateStr, "N/D");
  } else {
    sprintf(closeDateStr, "%02d/%02d/%04d %02d:%02d", ticket.closedAt.day,
            ticket.closedAt.month, ticket.closedAt.year, ticket.closedAt.hour,
            ticket.closedAt.min);
  }

  if (ticket.estimatedConclusion.day == 0 &&
      ticket.estimatedConclusion.month == 0 &&
      ticket.estimatedConclusion.year == 0) {
    strcpy(estimatedConclusionDateStr, "N/D");
  } else {
    sprintf(estimatedConclusionDateStr, "%02d/%02d/%04d %02d:%02d",
            ticket.estimatedConclusion.day, ticket.estimatedConclusion.month,
            ticket.estimatedConclusion.year, ticket.estimatedConclusion.hour,
            ticket.estimatedConclusion.min);
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
  printf("%-12s %-18d\n", "SLA:", getSLA(ticket.priority));

  printf("---------------------------------------------------------------\n");
  printf("Descricao:   %s\n", ticket.description);
}
