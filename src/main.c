#include "funcoes.h"
#include "input/input.h"
#include "logs/log.h"
#include "persistencia.h"
#include "ui/ui.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void menuAdminFilter(const char *username) {
  int filterOption;
  int filterValue; // Guardar o valor do filtro escolhido
  int running = 0;

  do {
    system("cls");
    Navbar();
    Title("FILTRAR TICKET", "⌕");
    puts("1  - Filtrar tickets por prioridade");
    puts("2  - Filtrar tickets por tipo");
    puts("3  - Filtrar tickets por estado");
    puts("0  - Voltar");
    filterOption = readInt("Opção: ");

    switch (filterOption) {
    case 1:
      filterValue = readIntRange(
          "\nQual a prioridade? (1-Baixa, 2-Media, 3-Alta, 4-Critica): ", 1, 4);
      listTicketsByFilter(filterValue, 'P');
      logEvent(username, "FILTRAR_TICKETS", "Filtro por prioridade");

      waitForKey();
      break;

    case 2: {
      int realId;
      int displayId;

      do {
        int max = getTicketTypeCount();
        puts("\nTipos disponíveis:");
        listTicketTypes();

        displayId = readIntRange("Opção: ", 1, max);

        realId = getRealTypeId(displayId);

        if (realId == -1) {
          puts("Opção inválida.");
        }

      } while (realId == -1);

      listTicketsByFilter(realId, 'T');
      logEvent(username, "FILTRAR_TICKETS", "Filtro por tipo");

      waitForKey();
      break;
    }
    case 3:
      filterValue = readIntRange(
          "Qual o estado? (1-Aberto, 2-Em Atendimento, 3-Espera User, "
          "4-Resolvido, 5-Fechado): ",
          1, 5);
      listTicketsByFilter(filterValue, 'S');
      logEvent(username, "FILTRAR_TICKETS", "Filtro por estado");
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

void menuAdminOrder(const char *username) {

  int orderOption;
  int running = 0;

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
      logEvent(username, "ORDENAR_TICKETS", "Ordenacao por data");
      break;
    case 2:
      sortTicketsByPriority();
      listAllTickets();
      logEvent(username, "ORDENAR_TICKETS", "Ordenacao por prioridade");
      break;
    case 3:
      sortTicketsByTechnician();
      listAllTickets();
      logEvent(username, "ORDENAR_TICKETS", "Ordenacao por tecnico");
      break;
    case 4:
      sortTicketsById();
      listAllTickets();
      logEvent(username, "ORDENAR_TICKETS", "Ordenacao por ID");
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

void menuManagmentTypes(const char *username) {
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
      logEvent(username, "LISTAR_CATEGORIAS", "Categorias listadas");
      waitForKey();
      break;
    case 2: {
      TICKET_TYPE ticketType;
      do {
        readString("\nInsira o nome do novo tipo: ", ticketType.name,
                   sizeof(ticketType.name));
      } while (validateStringLength(ticketType.name, 3,
                                    sizeof(ticketType.name) - 1) == -1);
      if (createTicketType(ticketType) == 0) {
        logSuccess(username, "CRIAR_CATEGORIA", ticketType.name);
        puts("\nSUCESSO: Tipo de ticket criado com sucesso.");
      } else {
        logError(username, "CRIAR_CATEGORIA", ticketType.name);
        puts("\nERRO: Não foi possível criar o tipo de ticket.");
      }
      waitForKey();
      break;
    }
    case 3: {
      int displayId;

      if (listTicketTypes() == -1) {
        puts("ERRO: Nenhuma categoria para editar.");
        waitForKey();
        break;
      }
      displayId = readInt("\nIntroduza o #ID do ticket que deseja editar: ");
      int realId = getRealTypeId(displayId);

      if (realId == -1) {
        logError(username, "EDITAR_CATEGORIA", "Categoria invalida");
        puts("Opção inválida.");
      } else {
        if (updateTicketType(realId) == 0) {
          logSuccess(username, "EDITAR_CATEGORIA", "Categoria atualizada");
          puts("\nSUCESSO: Tipo de ticket atualizado com sucesso.");
        } else {
          logError(username, "EDITAR_CATEGORIA", "Erro ao atualizar categoria");
          puts("\nERRO: Não foi possível atualizar o tipo de ticket.");
        }
      }
      waitForKey();
      break;
    }
    case 4: {
      int displayId;
      puts("\n=== REMOVER CATEGORIA ===");

      if (listTicketTypes() == -1) {
        puts("ERRO: Nenhuma categoria para remover.");
        waitForKey();
        break;
      }

      displayId = readInt("\nIntroduza o #ID do ticket que deseja eliminar: ");
      int realId = getRealTypeId(displayId);

      if (realId == -1) {
        logError(username, "REMOVER_CATEGORIA", "Categoria invalida");
        puts("Opção inválida.");
      } else {
        if (deleteType(realId) == 0) {
          logSuccess(username, "REMOVER_CATEGORIA", "Categoria removida");
          puts("\nSUCESSO: Tipo de ticket removido com sucesso.");
        } else {
          logError(username, "REMOVER_CATEGORIA", "Erro ao remover categoria");
          puts("\nERRO: Não foi possível remover o tipo de ticket.");
        }
      }
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

void adminMenu(int *alertSLA, int logged_userId, const char *username) {
  int option;
  do {
    system("cls");
    Navbar();
    Title("PAINEL ADMIN", "🔑");
    puts("1  - Adicionar ticket");
    puts("2  - Editar ticket");
    puts("3  - Listar todos os tickets");
    puts("4  - Ver ticket por ID");
    puts("5  - Remover ticket");
    puts("6  - Filtrar tickets");
    puts("7  - Ordenar tickets");
    puts("8  - Adicionar/Remover/Editar categorias");
    puts("9  - Listar tecnicos");
    puts("10 - Validar tecnicos pendentes"); // aqui no 10 esta correto
    puts("11 - Tickets sem tecnicos associados");
    puts("12 - Atribuir tecnico a ticket");
    puts("13 - Ver historico de um ticket");
    puts("14 - Tempo medio de resolucao por tecnico");
    puts("15 - Tempo medio de resolucao por categoria");
    puts("16 - Gerar relatorio");
    puts("17 - Alertas de tickets fora do SLA");
    puts("0  - Logout");
    puts("====================================");
    option = readInt("Opção: ");

    switch (option) {
    case 1: {
      system("cls");
      Navbar();
      Title("CRIAR TICKET", "➕");
      TICKET_INFO ticket;
      int realId, displayId, existTypes = 0;
      do {
        puts("\nTipos disponíveis:");

        if (listTicketTypes() == -1) {
          puts("ERRO: Nenhuma categoria disponível.");
          existTypes = -1;
          waitForKey();
          break;
        }

        int max = getTicketTypeCount();

        displayId = readIntRange("Opção: ", 1, max);

        realId = getRealTypeId(displayId);

        if (realId == -1) {
          puts("Opção inválida.");
        }

      } while (realId == -1);

      if (existTypes == -1) {
        break;
      }

      ticket.typeId = realId;

      do {
        readString("Descricao: ", ticket.description,
                   sizeof(ticket.description));
      } while (validateStringLength(ticket.description, 5,
                                    sizeof(ticket.description) - 1) == -1);

      ticket.priority = readIntRange(
          "Prioridade (1-Baixa, 2-Media, 3-Alta, 4-Critica): ", 1, 4);

      do {
        readString("Utilizador que reportou: ", ticket.user, MAX_STR);
      } while (validateStringLength(ticket.user, 3, MAX_STR - 1) == -1);

      if (createTicket(ticket) == 0) {
        char details[160];
        snprintf(details, sizeof(details), "Ticket criado para %s",
                 ticket.user);
        logSuccess(username, "CRIAR_TICKET", details);
        puts("\nSUCESSO: Ticket criado com sucesso.");
      } else {
        logError(username, "CRIAR_TICKET", "Erro ao criar ticket");
        puts("\nERRO: Não foi possível criar o ticket.");
      }
      waitForKey();
      break;
    }
    case 2: {
      system("cls");
      Navbar();
      Title("EDITAR TICKET", "📝");

      if (listAllTickets() == -1) {
        puts("ERRO: Nenhum ticket para editar.");
        waitForKey();
        break;
      }

      int ticketId;
      ticketId = readInt("Introduza o #ID do ticket que deseja editar: ");
      if (updateTicket(ticketId, logged_userId) == 0) {
        char details[80];
        snprintf(details, sizeof(details), "Ticket #%d atualizado", ticketId);
        logSuccess(username, "EDITAR_TICKET", details);
        printf("\nSUCESSO: Ticket #%d atualizado com sucesso!\n", ticketId);
      } else {
        logError(username, "EDITAR_TICKET", "Erro ao atualizar ticket");
        printf("\nERRO: Não foi possível atualizar o ticket #%d.\n", ticketId);
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

      if (listAllTickets() == -1) {
        puts("ERRO: Nenhum ticket para visualizar.");
        waitForKey();
        break;
      }

      int ticketId = 0;
      ticketId = readInt("Digita o id do ticket a visualizar: ");
      if (showTicketById(ticketId) == -1) {
        logError(username, "VER_TICKET", "Ticket nao encontrado");
        puts("\nERRO: Não foi possível encontrar o ticket.");
      }
      waitForKey();
      break;
    }
    case 5: {
      system("cls");
      Navbar();
      Title("ELIMINAR TICKET", "🚫");
      if (listAllTickets() == -1) {
        puts("ERRO: Nenhum ticket para remover.");
        waitForKey();
        break;
      }
      int ticketId;
      ticketId = readInt("Introduza o ticket que deseja remover: ");
      if (deleteTicket(ticketId) == 0) {
        char details[80];
        snprintf(details, sizeof(details), "Ticket #%d removido",
                 ticketId); // Para guardar no log
        logSuccess(username, "REMOVER_TICKET", details);
        printf("\nSUCESSO: Ticket #%d removido com sucesso.\n", ticketId);
      } else {
        logError(username, "REMOVER_TICKET", "Erro ao remover ticket");
        printf("\nERRO: Não foi possível remover o ticket #%d.\n", ticketId);
      }
      waitForKey();
      break;
    }
    case 6:
      menuAdminFilter(username);
      waitForKey();
      break;
    case 7:
      menuAdminOrder(username);
      waitForKey();
      break;
    case 8:
      menuManagmentTypes(username);
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
        if (validateTechnician(userId) == 0) {
          char details[80];
          snprintf(details, sizeof(details), "Tecnico #%d validado", userId);
          logSuccess(username, "VALIDAR_TECNICO", details);
          puts("\nSUCESSO: Técnico validado com sucesso.");
        } else {
          logError(username, "VALIDAR_TECNICO", "Erro ao validar tecnico");
          puts("\nERRO: Não foi possível validar o técnico.");
        }
      }
      waitForKey();
      break;
    }
    case 11:
      system("cls");
      Navbar();
      Title("TICKETS SEM TECNICOS", "📊");
      listPendentTickts();
      waitForKey();
      break;
    case 12: {
      system("cls");
      Navbar();
      Title("ATRIBUIR TECNICOS", "📝");
      int ticketId, tecnicoId;
      if (listAllTickets() == -1) {
        puts("ERRO: Nenhum ticket para atribuir.");
        waitForKey();
        break;
      }
      ticketId = readInt("ID do ticket: ");
      system("cls");
      Navbar();
      Title("ATRIBUIR TECNICOS", "📝");
      listAllTechnicians();
      tecnicoId = readInt("ID do tecnico: ");

      if (assignTechnician(ticketId, tecnicoId) == 0) {
        char details[120];
        snprintf(details, sizeof(details), "Ticket #%d -> Tecnico #%d",
                 ticketId, tecnicoId);
        logEvent(username, "ATRIBUIR_TECNICO", details);
        printf("\nSUCESSO: Técnico #%d atribuído ao ticket #%d com sucesso.",
               tecnicoId, ticketId);
      } else {
        logError(username, "ATRIBUIR_TECNICO", "Erro ao atribuir tecnico");
        printf("\nERRO: Não foi possível atribuir o técnico #%d ao ticket #%d.",
               tecnicoId, ticketId);
      }
      waitForKey();
      break;
    }
    case 13: {
      system("cls");
      Navbar();
      Title("HISTORICO TICKETS", "📜");
      listAllTickets();
      int id;
      id = readInt("ID do ticket: ");
      printTicketHistory(id);
      logEvent(username, "VER_HISTORICO", "Historico de ticket consultado");
      waitForKey();
      break;
    }
    case 14:
      system("cls");
      Navbar();
      Title("TEMPO MEDIO", "📊");
      averageTimePerTechnician();
      logEvent(username, "TEMPO_MEDIO", "Tempo medio por tecnico consultado");
      waitForKey();
      break;
    case 15:
      system("cls");
      Navbar();
      Title("TEMPO MEDIO", "📊");
      averageTimePerType();
      logEvent(username, "TEMPO_MEDIO", "Tempo medio por categoria consultado");
      waitForKey();
      break;
    case 16: {

      int optionReport;
      int isRunning = 0;

      do {
        system("cls");
        Navbar();
        Title("RELATORIO", "📋");
        puts("Tipo de relatório:");
        puts("1 - Mensal Estatístico");
        puts("2 - Semanal Estatístico");
        puts("3 - Pedidos registados e resolvidos");
        puts("0 - Voltar");
        optionReport = readInt("Opção: ");

        switch (optionReport) {
        case 1: {
          int month;
          int year;
          char dateInput[20];

          do {
            readString("Introduza o mês e ano (mm/YYYY): ", dateInput,
                       sizeof(dateInput));
          } while (validateStringLength(dateInput, 7, 7) == -1);

          if (sscanf(dateInput, "%d/%d", &month, &year) != 2) {
            logError(username, "GERAR_RELATORIO", "Formato mensal invalido");
            puts("\nERRO: Formato invalido.");
            break;
          }

          if (generateMonthReport(month, year) == 0) {
            logSuccess(username, "GERAR_RELATORIO", "Relatorio mensal gerado");
            puts("\nSUCESSO: Relatório gerado com sucesso.");
          } else {
            logError(username, "GERAR_RELATORIO",
                     "Erro ao gerar relatorio mensal");
            puts("\nERRO: Não foi possível gerar relatório.");
          }
          isRunning = 1;
          waitForKey();

          break;
        }
        case 2: {
          DateTime startDate;
          char dateInput[20];

          do {
            readString("Introduza o dia, mês e ano (dd/mm/YYYY): ", dateInput,
                       sizeof(dateInput));
          } while (validateStringLength(dateInput, 10, 10) == -1);
          if (sscanf(dateInput, "%d/%d/%d", &startDate.day, &startDate.month,
                     &startDate.year) != 3) {
            logError(username, "GERAR_RELATORIO", "Formato semanal invalido");
            puts("\nERRO: Formato invalido.");
            break;
          }

          if (generateWeeklyReport(startDate) == 0) {
            logSuccess(username, "GERAR_RELATORIO", "Relatorio semanal gerado");
            puts("\nSUCESSO: Relatório gerado com sucesso.");
          } else {
            logError(username, "GERAR_RELATORIO",
                     "Erro ao gerar relatorio semanal");
            puts("\nERRO: Não foi possível gerar relatório.");
          }

          isRunning = 1;

          waitForKey();
          break;
        }
        case 3: {
          if (createPeriodicReports() == 0) {
            logSuccess(username, "GERAR_RELATORIO",
                       "Relatorio periodico gerado");
            puts("\nSUCESSO: Relatório gerado com sucesso.");
          } else {
            logError(username, "GERAR_RELATORIO",
                     "Erro ao gerar relatorio periodico");
            puts("\nERRO: Não foi possível gerar relatório.");
          }
        } break;
        case 0: {
          isRunning = 1;
          break;
        }
        default: {
          puts("Opção invalida.");
          break;
        }
        }

      } while (!isRunning);

      break;
    }
    case 17:
      system("cls");
      Navbar();
      Title("ALERTAS", "👁️");
      if (alertTicketSLA(alertSLA) == 0) {
        logSuccess(username, "ALERTAS_SLA", "Alertas SLA gerados");
        puts("\nSUCESSO: Alerta gerado com sucesso.");
      } else {
        logError(username, "ALERTAS_SLA", "Erro ao gerar alertas SLA");
        puts("\nERRO: Não foi possível gerar Alerta.");
      }
      waitForKey();
      break;
    case 0:
      logEvent(username, "LOGOUT", "Logout do administrador");
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
    option = readInt("Opção: ");

    switch (option) {
    case 1:
      showTicketByTechnician(logged_userId);
      logEvent(username, "LISTAR_MEUS_TICKETS", "Tickets do tecnico listados");
      waitForKey();
      break;
    case 2: {
      int idTicket;
      showTicketPendentByTechnician(logged_userId);
      idTicket = readInt("\nID do ticket a aceitar (0 para cancelar): ");

      if (idTicket == 0) {
        logEvent(username, "ACEITAR_TICKET", "Operacao cancelada");
        puts("\nERRO: Operacao cancelada.");
        waitForKey();
        break;
      }

      if (acceptTicket(logged_userId, idTicket) == 0) {
        char details[80];
        snprintf(details, sizeof(details), "Ticket #%d aceite", idTicket);
        logSuccess(username, "ACEITAR_TICKET", details);
        printf("\nSUCESSO: Ticket #%d aceite com sucesso.", idTicket);
      } else {
        logError(username, "ACEITAR_TICKET", "Erro ao aceitar ticket");
        printf("\nERRO: Não foi possível aceitar ticket #%d.", idTicket);
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
        logEvent(username, "ATUALIZAR_ESTADO", "Operacao cancelada");
        waitForKey();
        break;
      }

      if (updateTicketStatus(ticketId, logged_userId) == 0) {
        char details[80];
        snprintf(details, sizeof(details), "Ticket #%d atualizado", ticketId);
        logSuccess(username, "ATUALIZAR_ESTADO", details);
        printf("\nSUCESSO: Estado do Ticket #%d atualizado com sucesso.",
               ticketId);
      } else {
        logError(username, "ATUALIZAR_ESTADO", "Erro ao atualizar estado");
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
        logEvent(username, "ADICIONAR_COMENTARIO", "Operacao cancelada");
        puts("\nERRO: Operação cancelada.");
        waitForKey();
        break;
      }

      if (addComment(idTicket, logged_userId) == 0) {
        char details[80];
        snprintf(details, sizeof(details), "Comentario no ticket #%d",
                 idTicket);
        logSuccess(username, "ADICIONAR_COMENTARIO", details);
        printf("\nSUCESSO: Comentário adicionado ao Ticket #%d com sucesso.",
               idTicket);
      } else {
        logError(username, "ADICIONAR_COMENTARIO",
                 "Erro ao adicionar comentario");
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
        char details[80];
        snprintf(details, sizeof(details), "Ticket #%d delegado", idTicket);
        logSuccess(username, "DELEGAR_TICKET", details);
        printf("\nSUCESSO: Ticket #%d delegado ao técnico com sucesso.",
               idTicket);
      } else {
        logError(username, "DELEGAR_TICKET", "Erro ao delegar ticket");
        printf("\nERRO: Não foi possível delegar ticket #%d.", idTicket);
      }

      waitForKey();
      break;
    }
    case 6:
      if (exportTicketsCSV(username, logged_userId) == 0) {
        logSuccess(username, "EXPORTAR_CSV", "Tickets exportados para CSV");
        printf("\nSUCESSO: Tickets exportados para CSV com sucesso.");
      } else {
        logError(username, "EXPORTAR_CSV", "Erro ao exportar tickets para CSV");
        printf("\nERRO: Não foi possível exportar tickets para CSV.");
      }

      waitForKey();
      break;
    case 10:
      if (*alertSLA == 0) {
        printAlertsSLA();
        logEvent(username, "CONSULTAR_ALERTAS", "Alertas SLA consultados");
      }
      waitForKey();
      break;
    case 0:
      logEvent(username, "LOGOUT", "Logout do tecnico");
      printf("Logout...\n");

      break;
    default:
      printf("Opção invalida!\n");
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

  if (loadAllData() == 0) {
    logEvent("SISTEMA", "LOAD", "Dados carregados");
  } else {
    logError("SISTEMA", "LOAD",
             "Dados carregados parcialmente ou ficheiros inexistentes");
  }
  inicializarIds_Ticket();
  inicializarIds_TicketType();
  inicializarIds_Users();

  if (getUserCount() == 0) {
    if (createAdmin() != 0) {
      logError("SISTEMA", "CRIAR_ADMIN", "Erro ao criar administrador");
      puts("\nERRO: Não foi possível criar o utilizador administrador.");
    }
  }

  if (checkAdminValidated() == -1) {
    needNewPass = 0;
  }

  if (getTicketCount() == 0) {
    seederTypes();
    seederTickets();
    logEvent("SISTEMA", "SEED", "Dados iniciais criados");
  }

  while (1) {
    system("cls");
    Navbar();
    Title("MENU", "🎫");
    puts("1 - Login");
    puts("2 - Registar");
    puts("3 - Alterar PW");
    puts("0 - Sair");
    option = readInt("Opção: ");
    system("cls");

    if (option == 0)
      break;

    switch (option) {
    case 1:
      Navbar();
      Title("LOGIN", "🔐");
      do {
        readString("Username: ", username, MAX_STR);
      } while (validateStringLength(username, 3, MAX_STR - 1) == -1);

      do {
        readString("Password: ", pass, MAX_STR);
      } while (validateStringLength(pass, 5, MAX_STR - 1) == -1);
      int logged_userId = -1;

      int perfil = login(username, pass, &logged_userId);

      if (perfil == -1) {
        logError(username, "LOGIN", "Credenciais invalidas");
        printf("\nERRO: Credenciais invalidas!\n");
        if (userExistsByUsername(username) != 0) {
          int registerOption =
              readIntRange("Deseja criar conta? (1-Sim / 0-Nao): ", 0, 1);

          if (registerOption == 1) {
            system("cls");
            Navbar();
            Title("REGISTAR", "📝");
            do {
              readString("Nome: ", user.name, MAX_STR);
            } while (validateStringLength(user.name, 3, MAX_STR - 1) == -1);

            do {
              readString("Username: ", user.username, MAX_STR);
            } while (validateStringLength(user.username, 3, MAX_STR - 1) == -1);

            do {
              readString("Password: ", user.password, MAX_STR);
            } while (validateStringLength(user.password, 5, MAX_STR - 1) == -1);

            user.perfil = PERFIL_TECNICO;

            if (registerUser(user) == 0) {
              logSuccess(user.username, "REGISTAR", "Utilizador criado");
              puts("\nSUCESSO: Utilizador criado com sucesso");
            } else {
              logError(user.username, "REGISTAR", "Registo mal sucedido");
              printf("\nERRO: Registo mal sucedido\n");
            }
            system("cls");
          }
        } else {
          logError(username, "LOGIN", "Password invalida");
          puts("\nERRO: Password invalida.");
        }
        waitForKey();
      } else if (perfil == -2) {
        logError(username, "LOGIN", "Tecnico ainda nao validado");
        puts("\nERRO: Técnico ainda não validado! Contacte o administrador de "
             "sistema\n");
        waitForKey();
      } else if (perfil == PERFIL_ADMIN) {
        logSuccess(username, "LOGIN", "Autenticado como administrador");
        if (isLogged == 1) {
          puts("Utilizador logado com sucesso");
        }
        waitForKey();
        system("cls");
        Navbar();
        Title("TROCAR PASSWORD", "🔄");

        if (isLogged != 1 && needNewPass == 0) {
          Navbar();
          puts("\n=== Primeiro login: altere a password! ===\n");
          do {
            readString("\nNova password: ", newPassword, MAX_STR);

          } while ((validateStringLength(newPassword, 5, MAX_STR - 1)) == -1);

          if (changePassword(username, newPassword) == 0) {
            logSuccess(username, "ALTERAR_PW", "Password alterada");
            puts("Password alterada com sucesso!");
            isLogged = 1;
            puts("\nInicio de sessao automatico...");
            waitForKey();
            system("cls");
          } else {
            logError(username, "ALTERAR_PW", "Erro ao alterar password");
            puts("\nERRO: Ocorreu um erro ao alterar palavra-passe");
          }
        }
        adminMenu(&alertSLA, logged_userId, username);
      } else if (perfil == PERFIL_TECNICO) {
        logSuccess(username, "LOGIN", "Autenticado como tecnico");
        technicianMenu(username, logged_userId, &alertSLA);
      }
      system("cls");
      break;

    case 2:
      Navbar();
      Title("REGISTAR", "📝");
      do {
        readString("Nome: ", user.name, MAX_STR);
      } while (validateStringLength(user.name, 3, MAX_STR - 1) == -1);

      do {
        readString("Username: ", user.username, MAX_STR);
      } while (validateStringLength(user.username, 3, MAX_STR - 1) == -1);

      do {
        readString("Password: ", user.password, MAX_STR);
      } while (validateStringLength(user.password, 5, MAX_STR - 1) == -1);

      user.perfil = PERFIL_TECNICO;

      if (registerUser(user) == 0) {
        logSuccess(user.username, "REGISTAR", "Utilizador criado");
        puts("Utilizador criado com sucesso");
      } else {
        logError(user.username, "REGISTAR", "Registo mal sucedido");
        printf("\nERRO: Registo mal sucedido\n");
      }
      waitForKey();
      system("cls");
      break;

    case 3:
      Navbar();
      Title("ESQUECEU PASSWORD", "🔄");
      do {
        readString("Username: ", username, MAX_STR);
      } while (validateStringLength(username, 3, MAX_STR - 1) == -1);

      do {
        readString("Password: ", newPassword, MAX_STR);
      } while (validateStringLength(newPassword, 5, MAX_STR - 1) == -1);

      if (changePassword(username, newPassword) == 0) {
        logSuccess(username, "ALTERAR_PW", "Password alterada");
        puts("Password alterada com sucesso!");
      } else {
        logError(username, "ALTERAR_PW", "Erro ao alterar password");
        puts("\nERRO: Ocorreu um erro ao alterar palavra-passe");
      }
      waitForKey();
      system("cls");
      break;
    }
  }

  if (backupAllData() == 0) {
    logEvent("SISTEMA", "BACKUP", "Dados guardados");
  } else {
    logError("SISTEMA", "BACKUP", "Erro ao guardar dados");
  }
  fcloseall();
  cleanAllLists();
  return 0;
}
