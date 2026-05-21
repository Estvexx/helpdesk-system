#include "funcoes.h"
#include "input/input.h"
#include "persistencia.h"
#include "ui/ui.h"

#include <securitybaseapi.h>
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
    filterOption = readInt("Opção: ");

    switch (filterOption) {
    case 1:
      filterValue = readIntRange(
          "\nQual a prioridade? (1-Baixa, 2-Media, 3-Alta, 4-Critica): ", 1, 4);
      listTicketsByPriority(filterValue);

      waitForKey();
      break;

    case 2: {
      int realId;
      int displayId;

      do {
        puts("\nTipos disponíveis:");
        listTicketTypes();
        int max = getTicketTypeCount();

        displayId = readIntRange("Opção: ", 1, max);

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
      filterValue = readIntRange(
          "Qual o estado? (1-Aberto, 2-Em Atendimento, 3-Espera User, "
          "4-Resolvido, 5-Fechado): ",
          1, 5);
      listTicketsByStatus(filterValue);
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
    puts("1  - Ordenar tickets por data");
    puts("2  - Ordenar tickets por prioridade");
    puts("3  - Ordenar tickets por tecnico");
    puts("4  - Ordenar tickets por ID");
    puts("0  - Voltar");
    orderOption = readInt("Opção: ");
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
    case 4:
      sortTicketsById();
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
  int typeOption;
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
    typeOption = readInt("Opção: ");
    switch (typeOption) {

    case 1:
      puts("\n=== LISTAR TIPOS ===");
      listTicketTypes();
      waitForKey();
      break;
    case 2: {
      TICKET_TYPE ticketType;
      do {
        readString("\nInsira o nome do novo tipo: ", ticketType.name,
                   sizeof(ticketType.name));
      } while (charIsValid(ticketType.name, 3, sizeof(ticketType.name) - 1) ==
               -1);
      createTicketType(ticketType);
      waitForKey();
      break;
    }
    case 3: {
      int displayId;
      displayId = readInt("\nIntroduza o #ID do ticket que deseja editar: ");
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
      displayId = readInt("\nIntroduza o #ID do ticket que deseja eliminar: ");
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
      waitForKey();
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
    option = readInt("Opcao: ");

    switch (option) {
    case 1: {
      system("cls");
      Navbar();
      Title("CRIAR TICKET", "➕");
      TICKET_INFO ticket;
      int realId, displayId;
      do {
        puts("\nTipos disponíveis:");
        listTicketTypes();
        int max = getTicketTypeCount();

        displayId = readIntRange("Opção: ", 1, max);

        realId = getRealTypeId(displayId);

        if (realId == -1) {
          puts("\nOpção inválida.");
        }

      } while (realId == -1);

      ticket.typeId = realId;

      do {
        readString("Descricao: ", ticket.description,
                   sizeof(ticket.description));
      } while (charIsValid(ticket.description, 5,
                           sizeof(ticket.description) - 1) == -1);

      ticket.priority = readIntRange(
          "Prioridade (1-Baixa, 2-Media, 3-Alta, 4-Critica): ", 1, 4);

      do {
        readString("Utilizador que reportou: ", ticket.user, MAX_STR);
      } while (charIsValid(ticket.user, 3, MAX_STR - 1) == -1);

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
      ticketId = readInt("Introduza o #ID do ticket que deseja editar: ");
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
      listAllTickets();
      int ticketId = 0;
      ticketId = readInt("Digita o id do ticket a visualizar: ");
      showTicketById(ticketId);
      waitForKey();
      break;
    }
    case 5: {
      system("cls");
      Navbar();
      Title("ELIMINAR TICKET", "🗑️");
      listAllTickets();
      int ticketId;
      ticketId = readInt("Introduza o ticket que deseja remover: ");
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
      userId = readInt("ID do tecnico a validar (0 para cancelar): ");

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
      listAllTickets();
      ticketId = readInt("ID do ticket: ");
      system("cls");
      Navbar();
      Title("ATRIBUIR TECNICOS", "📝");
      listAllTechnicians();
      tecnicoId = readInt("ID do tecnico: ");

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
      id = readInt("ID do ticket: ");
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
        optionReport = readInt("Opcao: ");

        switch (optionReport) {
        case 1: {
          int month;
          int year;
          char dateInput[20];

          readString("Introduza o mês e ano (mm/YYYY): ", dateInput,
                     sizeof(dateInput));

          if (sscanf(dateInput, "%d/%d", &month, &year) != 2) {
            puts("Formato invalido.");
            break;
          }

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
          char dateInput[20];

          readString("Introduza o dia, mês e ano (dd/mm/YYYY): ", dateInput,
                     sizeof(dateInput));
          if (sscanf(dateInput, "%d/%d/%d", &startDate.day, &startDate.month,
                     &startDate.year) != 3) {
            puts("Formato invalido.");
            break;
          }

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
          if (createPeriodicReports() == 0) {
            puts("SUCESSO: Relatório gerado com sucesso.");
          } else {
            puts("ERRO: Não foi possível gerar relatório.");
          }

        } break;
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
    option = readInt("Opcao: ");

    switch (option) {
    case 1:
      showTicketByTechnician(logged_userId);
      waitForKey();
      break;
    case 2: {
      int idTicket;
      showTicketPendentByTechnician(logged_userId);
      idTicket = readInt("\nID do ticket a aceitar (0 para cancelar): ");

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
      showTicketByTechnician(logged_userId);
      int ticketId;
      ticketId = readInt("\nIntroduza o ID do ticket que deseja atualizar o "
                         "estado: (0 para cancelar): ");

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

      idTicket = readInt("\nID do ticket para comentar (0 para cancelar): ");

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
      showTicketByTechnician(logged_userId);
      int idTicket;
      idTicket = readInt("ID do ticket a delegar: ");

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

  if (getUserCount() == 0) {
    createAdmin();
  }

  if (checkAdminValidated() == -1) {
    needNewPass = 0;
  }

  if (getTicketCount() == 0) {
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
    option = readInt("Opcao: ");
    system("cls");

    if (option == 0)
      break;

    switch (option) {
    case 1:
      Navbar();
      Title("LOGIN", "🔐");
      do {
        readString("Username: ", username, MAX_STR);
      } while (charIsValid(username, 3, MAX_STR - 1) == -1);

      do {
        readString("Password: ", pass, MAX_STR);
      } while (charIsValid(pass, 5, MAX_STR - 1) == -1);
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
            readString("\nNova password: ", newPassword, MAX_STR);

          } while ((charIsValid(newPassword, 5, MAX_STR)) == -1);

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
      do {
        readString("Nome: ", user.name, MAX_STR);
      } while (charIsValid(user.name, 3, MAX_STR - 1) == -1);

      do {
        readString("Username: ", user.username, MAX_STR);
      } while (charIsValid(user.username, 3, MAX_STR - 1) == -1);

      do {
        readString("Password: ", user.password, MAX_STR);
      } while (charIsValid(user.password, 5, MAX_STR - 1) == -1);

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
      do {
        readString("Username: ", username, MAX_STR);
      } while (charIsValid(username, 3, MAX_STR - 1) == -1);

      do {
        readString("Password: ", newPassword, MAX_STR);
      } while (charIsValid(newPassword, 5, MAX_STR - 1) == -1);

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
