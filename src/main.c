#include "funcoes.h"
#include "persistencia.h"
#include "ui/ui.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void menuAdminFilter() {
  int filterOption;
  int filterValue; // Guardar o valor do filtro escolhido
  int running = 0;

  do {
    system("cls");
    Navbar();
    Title("FILTRAR TICKET", "🔎");
    puts("1  - Filtrar tickets por prioridade");
    puts("2  - Filtrar tickets por tipo");
    puts("3  - Filtrar tickets por estado");
    puts("0  - Voltar");
    printf("Opcao: ");
    scanf("%d", &filterOption);
    clearBuffer();

    switch (filterOption) {
    case 1:
      do {
        printf("\nQual a prioridade? (1-Baixa, 2-Media, 3-Alta, 4-Critica): ");
        if (scanf("%d", &filterValue) == 1 && filterValue >= 1 &&
            filterValue <= 4) {
          clearBuffer();
          listTicketsByPriority(filterValue);
          break;
        } else {
          clearBuffer();
          puts("\nOpção inválida!");
        }
      } while (1);

      waitForKey();
      break;

    case 2: {
      int realId;
      int displayId;

      do {
        puts("\nTipos disponíveis:");
        listTicketTypes();

        printf("\nOpção: ");
        scanf("%d", &displayId);
        clearBuffer();

        realId = getRealTypeId(displayId);

        if (realId == -1) {
          puts("\nOpção inválida.");
        }

      } while (realId == -1);

      listTicketsByType(realId);

      waitForKey();
      break;
    }
    case 3:
      do {
        printf("\nQual o estado? (1-Aberto, 2-Em Atendimento, 3-Espera User, "
               "4-Resolvido, 5-Fechado): ");

        if (scanf("%d", &filterValue) == 1 && filterValue >= 1 &&
            filterValue <= 5) {
          clearBuffer();
          listTicketsByStatus(filterValue);

          break;
        } else {
          clearBuffer();
          puts("\nOpção inválida!");
        }

      } while (1);

      waitForKey();
      break;

    case 0:
      running = 1;
      break;

    default:
      puts("Opção inválida!");
      waitForKey();
      break;
    }

  } while (!running);
}

void menuAdminOrder() {

  int orderOption;
  int running;

  do {
    system("cls");
    Navbar();
    Title("ORDENAR TICKETS", "🔃");
    puts("\n== ORDENAR ==");
    puts("1  - Ordenar tickets por data");
    puts("2  - Ordenar tickets por prioridade");
    puts("3  - Ordenar tickets por tecnico");
    puts("0  - Voltar");
    printf("Opcao: ");
    scanf("%d", &orderOption);
    clearBuffer();
    switch (orderOption) {

    case 1:
      sortTicketsByDate();
      listAllTickets();
      break;
    case 2:
      sortTicketsByPriority();
      listAllTickets();
      break;
    case 3:
      sortTicketsByTechnician();
      listAllTickets();
      break;
    case 0:
      running = 1;
      break;
    default:
      puts("Opção inválida!");
      break;
    }

  } while (!running);
}

void menuManagmentTypes() {
  int orderOption;
  int running = 0;

  do {
    system("cls");
    Navbar();
    Title("MENU CATEGORIAS", "🔃");
    puts("1  - Listar");
    puts("2  - Adicionar");
    puts("3  - Editar");
    puts("4  - Remover");
    puts("0  - Voltar");
    printf("Opcao: ");
    scanf("%d", &orderOption);
    clearBuffer();
    switch (orderOption) {

    case 1:
      puts("\n=== LISTAR TIPOS ===");
      listTicketTypes();
      waitForKey();
      break;
    case 2: {
      TICKET_TYPE ticketType;
      puts("\nInsira o nome do novo tipo: ");
      fgets(ticketType.name, sizeof(ticketType.name), stdin);
      ticketType.name[strcspn(ticketType.name, "\n")] = 0;
      createTicketType(ticketType);
      waitForKey();
      break;
    }
    case 3: {
      int displayId;
      puts("\nIntroduza o #ID do ticket que deseja editar: ");
      scanf("%d", &displayId);
      clearBuffer();
      int realId = getRealTypeId(displayId);

        if (realId == -1) {
          puts("\nOpção inválida.");
        }
      updateTicketType(realId);
      waitForKey();
      break;
    }
    case 4: {
      int displayId;
      puts("\nIntroduza o #ID do ticket que deseja eliminar: ");
      scanf("%d", &displayId);
      clearBuffer();
      int realId = getRealTypeId(displayId);

        if (realId == -1) {
          puts("\nOpção inválida.");
        }
      deleteType(realId);
      waitForKey();
      break;
    }
    case 0:
      running = 1;
      break;
    default:
      puts("Opção inválida!");
      break;
    }

  } while (!running);
}

void adminMenu(int *alertSLA, int logged_userId) {
  int option;
  do {
    system("cls");
    Navbar();
    Title("PAINEL ADMIN", "⚙️");
    puts("1  - Adicionar ticket");
    puts("2  - Editar ticket");
    puts("3  - Listar todos os tickets");
    puts("4  - Ver ticket por ID");
    puts("5  - Remover ticket");
    puts("6  - Filtrar tickets");
    puts("7  - Ordenar tickets");
    puts("8  - Adicionar/Remover/Editar categorias");
    puts("9  - Listar tecnicos");
    puts("10 - Validar tecnicos pendentes");
    puts("11 - Atribuir tecnico a ticket");
    puts("12 - Ver historico de um ticket");
    puts("13 - Tempo medio de resolucao por tecnico");
    puts("14 - Tempo medio de resolucao por categoria");
    puts("15 - Gerar relatorio");
    puts("16 - Alertas de tickets fora do SLA");
    puts("0  - Logout");
    puts("====================================");
    printf("Opcao: ");
    scanf("%d", &option);
    clearBuffer();

    switch (option) {
    case 1: {
      system("cls");
      Navbar();
      Title("CRIAR TICKET", "➕");
      TICKET_INFO ticket;
      int realId, displayId;
      do {
        puts("Tipos disponíveis:");
        listTicketTypes();

        printf("\nOpção: ");

        if (scanf("%d", &displayId) != 1) {
          puts("\nEntrada inválida.");
          continue;
        }

        clearBuffer();

        realId = getRealTypeId(displayId);

        if (realId == -1) {
          puts("\nOpção inválida.");
        }

      } while (realId == -1);

      ticket.typeId = realId;

      printf("Descricao: ");
      fgets(ticket.description, 500, stdin);
      ticket.description[strcspn(ticket.description, "\n")] = 0;

      do {
        printf("Prioridade (1-Baixa, 2-Media, 3-Alta, 4-Critica): ");
        if (scanf("%d", &ticket.priority) != 1 || ticket.priority < 1 ||
            ticket.priority > 4) {
          clearBuffer();
          puts("Prioridade inválida!");
        } else {
          clearBuffer();
          break;
        }
      } while (1);

      printf("Utilizador que reportou: ");
      fgets(ticket.user, MAX_STR, stdin);
      ticket.user[strcspn(ticket.user, "\n")] = 0;

      createTicket(ticket);
      waitForKey();
      break;
    }
    case 2: {
      system("cls");
      Navbar();
      Title("EDITAR TICKET", "✏️");
      listAllTickets();
      int ticketId;
      printf("Introduza o #ID do ticket que deseja editar: ");
      scanf("%d", &ticketId);
      clearBuffer();
      if (updateTicket(ticketId, logged_userId) == 0) {
        printf("Ticket #%d atualizado com sucesso!\n", ticketId);
      } else {
        printf("Erro ao atualizar o ticket #%d.\n", ticketId);
      }
      waitForKey();
      break;
    }
    case 3: {
      system("cls");
      Navbar();
      Title("LISTAR TICKETS", "📋");
      listAllTickets();
      waitForKey();
      break;
    }
    case 4: {
      system("cls");
      Navbar();
      Title("VER TICKET", "🎫");
      int ticketId = 0;
      printf("Digita o id do ticket a visualizar: ");
      scanf("%i", &ticketId);
      clearBuffer();
      showTicketById(ticketId);
      waitForKey();
      break;
    }
    case 5: {
      system("cls");
      Navbar();
      Title("ELIMINAR TICKET", "🗑️");
      int ticketId;
      printf("Introduza o ticket que deseja remover: ");
      scanf("%d", &ticketId);
      clearBuffer();
      deleteTicket(ticketId);
      waitForKey();
      break;
    }
    case 6:
      menuAdminFilter();
      waitForKey();
      break;
    case 7:
      menuAdminOrder();
      waitForKey();
      break;
    case 8:
      menuManagmentTypes();
      waitForKey();
      break;
    case 9:
      system("cls");
      Navbar();
      Title("TECNICOS", "📋");
      listAllTechnicians();
      waitForKey();
      break;
    case 10: {
      system("cls");
      Navbar();
      Title("VALIDAR TECNICOS", "✅");
      listPendingTechnicians();

      int userId;
      printf("ID do tecnico a validar (0 para cancelar): ");
      scanf("%d", &userId);
      clearBuffer();

      if (userId != 0) {
        validateTechnician(userId);
      }
      waitForKey();
      break;
    }
    case 11: {
      system("cls");
      Navbar();
      Title("ATRIBUIR TECNICOS", "📝");
      int ticketId, tecnicoId;
      printf("ID do ticket: ");
      scanf("%d", &ticketId);
      clearBuffer();

      printf("ID do tecnico: ");
      scanf("%d", &tecnicoId);
      clearBuffer();

      if (assignTechnician(ticketId, tecnicoId) == 0) {
        printf("SUCESSO: Técnico #%d atribuído ao ticket #%d com sucesso.",
               tecnicoId, ticketId);
      } else {
        printf("ERRO: Não foi possível atribuir o técnico #%d ao ticket #%d.",
               tecnicoId, ticketId);
      }
      waitForKey();
      break;
    }
    case 12: {
      system("cls");
      Navbar();
      Title("HISTORICO TICKETS", "📜");
      int id;
      printf("ID do ticket: ");
      scanf("%d", &id);
      clearBuffer();
      printTicketHistory(id);
      waitForKey();
      break;
    }
    case 13:
      system("cls");
      Navbar();
      Title("TEMPO MÉDIO", "⏳");
      averageTimePerTechnician();
      waitForKey();
      break;
    case 14:
      system("cls");
      Navbar();
      Title("TEMPO MÉDIO", "⏳");
      averageTimePerType();
      waitForKey();
      break;
    case 15: {
      system("cls");
      Navbar();
      Title("RELATORIO", "📋");
      int optionReport;
      int isRunning = 0;

      do {
        puts("Tipo de relatório:");
        puts("1 - Mensal Estatístico");
        puts("2 - Semanal Estatístico");
        puts("3 - Pedidos registados e resolvidos");
        puts("0 - Voltar");
        printf("Opcao: ");

        scanf("%d", &optionReport);

        clearBuffer();

        switch (optionReport) {
        case 1: {
          int month;
          int year;

          printf("Introduza o mês e ano (mm/YYYY): ");

          if (scanf("%d/%d", &month, &year) != 2) {
            clearBuffer();
            puts("Formato invalido.");
            break;
          }

          clearBuffer();

          if (generateMonthReport(month, year) == -1) {
            puts("ERRO: Não foi possível gerar relatório.");
          } else {
            puts("SUCESSO: Relatório gerado com sucesso.");
          }
          isRunning = 1;
          waitForKey();

          break;
        }
        case 2: {
          DateTime startDate;

          printf("Introduza o dia, mês e ano (dd/mm/YYYY): ");
          if (scanf("%d/%d/%d", &startDate.day, &startDate.month,
                    &startDate.year) != 3) {
            clearBuffer();
            puts("Formato invalido.");
            break;
          }
          clearBuffer();

          if (generateWeeklyReport(startDate) == 0) {
            puts("SUCESSO: Relatório gerado com sucesso.");
          } else {
            puts("ERRO: Não foi possível gerar relatório.");
          }
          isRunning = 1;

          waitForKey();
          break;
        }
        case 3: {
          if(createPeriodicReports() == 0) {
            puts("SUCESSO: Relatório gerado com sucesso.");
          } else {
            puts("ERRO: Não foi possível gerar relatório.");
          }
          
        }
        break;
        case 0: {
          isRunning = 1;
          break;
        }
        default: {
          puts("Opcao invalida.");
          break;
        }
        }

      } while (!isRunning);

      break;
    }
    case 16:
      system("cls");
      Navbar();
      Title("ALERTAS", "👁️");
      puts("A criar alertas SLA .");
      if (alertTicketSLA(alertSLA) == 0) {
        puts("SUCESSO: Alerta gerado com sucesso.");
      } else {
        puts("ERRO: Não foi possível gerar Alerta.");
      }
      waitForKey();
      break;
    case 0:
      printf("Logout...\n");
      break;
    default:
      puts("Opção inválida!");
      waitForKey();
    }
  } while (option != 0);
}

void technicianMenu(char *username, int logged_userId, int *alertSLA) {
  int option;
  do {
    system("cls");
    Navbar();
    Title("MENU TECNICO", "👤");
    printf("Utilizador: %s\n\n", username);

    if (*alertSLA == 0) {
      printf("\nExistem alertas SLA!");
      puts("\n    10 - Consultar\n");
    }
    printf("1 - Ver os meus tickets\n");
    printf("2 - Aceitar ticket pendente\n");
    printf("3 - Atualizar estado de um ticket\n");
    printf("4 - Adicionar comentario/acao\n");
    printf("5 - Delegar ticket a outro tecnico\n");
    printf("6 - Exportar tickets para CSV\n");
    printf("0 - Logout\n");
    printf("==============================\n");
    printf("Opcao: ");
    scanf("%d", &option);
    clearBuffer();

    switch (option) {
    case 1:
      showTicketByTechnician(logged_userId);
      waitForKey();
      break;
    case 2: {
      int idTicket;
      showTicketByTechnician(logged_userId);
      do {
        printf("\nID do ticket a aceitar (0 para cancelar): ");
        if (scanf("%d", &idTicket) != 1) {
          clearBuffer();
          puts("\nEntrada inválida.");
        } else {
          clearBuffer();
          break;
        }
      } while (1);

      if (idTicket == 0) {
        puts("Operacao cancelada.");
        waitForKey();
        break;
      }

      if (acceptTicket(logged_userId, idTicket) == 0) {
        printf("SUCESSO: Ticket #%d aceite com sucesso.", idTicket);
      } else {
        printf("ERRO: Não foi possível aceitar ticket #%d.", idTicket);
      }

      waitForKey();
      break;
    }
    case 3: {
      int ticketId;
      printf("\nIntroduza o ID do ticket que deseja atualizar o estado: (0 "
             "para cancelar): ");
      scanf("%d", &ticketId);
      clearBuffer();

      if (ticketId == 0) {
        waitForKey();
        break;
      }

      if (updateTicketStatus(ticketId, logged_userId) == 0) {
        printf("\nSUCESSO: Estado do Ticket #%d atualizado com sucesso.",
               ticketId);
      } else {
        printf("\nERRO: Não foi possível alterar o estado do Ticket #%d.",
               ticketId);
      }

      waitForKey();
      break;
    }
    case 4: {
      int idTicket;

      showTicketByTechnician(logged_userId);

      puts("\nID do ticket para comentar (0 para cancelar): ");
      scanf("%d", &idTicket);
      clearBuffer();

      if (idTicket == 0) {
        puts("Operação cancelada.");
        waitForKey();
        break;
      }

      if (addComment(idTicket, logged_userId) == 0) {
        printf("\nSUCESSO: Comentário adicionado ao Ticket #%d com sucesso.",
               idTicket);
      } else {
        printf("\nERRO: Não foi possível adicionar comentário ao Ticket #%d.",
               idTicket);
      }
      waitForKey();
      break;
    }
    case 5: {
      int idTicket;
      system("cls");
      printf("ID do ticket a delegar: ");
      scanf("%d", &idTicket);
      clearBuffer();

      if (delegateTicket(idTicket, logged_userId) == 0) {
        printf("\nSUCESSO: Ticket #%d delegado ao técnico com sucesso.",
               idTicket);
      } else {
        printf("\nERRO: Não foi possível delegar ticket #%d.", idTicket);
      }

      waitForKey();
      break;
    }
    case 6:
      if (exportTicketsCSV(username, logged_userId) == 0) {
        printf("\nSUCESSO: Tickets exportados para CSV com sucesso.");
      } else {
        printf("\nERRO: Não foi possível exportar tickets para CSV.");
      }

      waitForKey();
      break;
    case 10:
      if (*alertSLA == 0) {
        printAlertsSLA();
      }
      waitForKey();
      break;
    case 0:
      printf("Logout...\n");

      break;
    default:
      printf("Opcao invalida!\n");
      waitForKey();
    }
  } while (option != 0);
}

int main() {
  system("chcp 65001 > nul");
  system("cls");
  char username[MAX_STR], pass[MAX_STR], newPassword[MAX_STR];
  int option;
  int isLogged = 0;
  int needNewPass = 1;
  USER_INFO user;

  int alertSLA = 1; // 0 - Tem alertas ; 1 - Sem alertas
  loadAllData();
  inicializarIds_Ticket();
  inicializarIds_TicketType();
  inicializarIds_Users();

  if(getUserCount() == 0) {
    createAdmin();
  }

  if (checkAdminValidated() == -1) {
    needNewPass = 0;
  }
  
  if(getTicketCount() == 0)  {
    seederTypes();
    seederTickets();
  }

  while (1) {
    system("cls");
    Navbar();
    Title("MENU", "🎫");
    puts("1 - Login");
    puts("2 - Registar");
    puts("3 - Alterar PW");
    puts("0 - Sair");
    printf("Opcao: ");
    scanf("%d", &option);
    clearBuffer();
    system("cls");

    if (option == 0)
      break;

    switch (option) {
    case 1:
      Navbar();
      Title("LOGIN", "🔐");
      puts("Username: ");
      fgets(username, MAX_STR, stdin);
      username[strcspn(username, "\n")] = 0;

      puts("Password: ");
      fgets(pass, MAX_STR, stdin);
      pass[strcspn(pass, "\n")] = 0;
      int logged_userId = -1;

      int perfil = login(username, pass, &logged_userId);

      if (perfil == -1) {
        printf("\nCredenciais invalidas!\n");
        waitForKey();
      } else if (perfil == -2) {
        puts("\nTécnico ainda não validado! Contacte o administrador de "
             "sistema\n");
        waitForKey();
      } else if (perfil == PERFIL_ADMIN) {
        if (isLogged == 1) {
          puts("Utilizador logado com sucesso");
        }
        waitForKey();
        system("cls");

        if (isLogged != 1 && needNewPass == 0) {
          Navbar();
          puts("\n=== Primeiro login: altere a password! ===\n");
          do {
            printf("\nNova password: ");
            fgets(newPassword, MAX_STR, stdin);
            newPassword[strcspn(newPassword, "\n")] = 0;

          } while((charIsValid(newPassword, 5, MAX_STR)) == -1);

          if (changePassword(username, newPassword) == 0) {
            puts("Password alterada com sucesso!");
            isLogged = 1;
            puts("\nInicio de sessao automatico...");
            waitForKey();
            system("cls");
          } else {
            puts("Ocorreu um erro ao alterar palavra-passe");
          }
        }
        adminMenu(&alertSLA, logged_userId);
      } else if (perfil == PERFIL_TECNICO) {
        technicianMenu(username, logged_userId, &alertSLA);
      }
      system("cls");
      break;

    case 2:
      Navbar();
      Title("REGISTAR", "📝");
      printf("Nome: ");
      fgets(user.name, MAX_STR, stdin);
      user.name[strcspn(user.name, "\n")] = 0;

      printf("Username: ");
      fgets(user.username, MAX_STR, stdin);
      user.username[strcspn(user.username, "\n")] = 0;

      printf("Password: ");
      fgets(user.password, MAX_STR, stdin);
      user.password[strcspn(user.password, "\n")] = 0;

      user.perfil = PERFIL_TECNICO;

      if (registerUser(user) == -1) {
        printf("ERRO: Registo mal sucedido\n");
        break;
      }
      puts("Utilizador criado com sucesso");
      waitForKey();
      system("cls");
      break;

    case 3:
      Navbar();
      Title("ESQUECEU PASSWORD", "🔄");
      printf("Username: ");
      fgets(username, MAX_STR, stdin);
      username[strcspn(username, "\n")] = 0;

      printf("Password: ");
      fgets(newPassword, MAX_STR, stdin);
      newPassword[strcspn(newPassword, "\n")] = 0;

      if (changePassword(username, newPassword) == 0) {
        puts("Password alterada com sucesso!");
      } else {
        puts("Ocorreu um erro ao alterar palavra-passe");
      }
      waitForKey();
      system("cls");
      break;
    }
  }

  backupAllData();
  fcloseall();
  cleanAllLists();
  return 0;
}