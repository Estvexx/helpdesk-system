#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void navbar()
{
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
void menuAdminFilter()
{
  int filterOption;
  int filterValue; // Guardar o valor do filtro escolhido
  int running = 0;

  do
  {
    puts("\n== FILTRAR TICKETS ==");
    puts("1  - Filtrar tickets por prioridade");
    puts("2  - Filtrar tickets por tipo");
    puts("3  - Filtrar tickets por estado");
    puts("0  - Voltar");
    printf("Opcao: ");
    scanf("%d", &filterOption);
    clearBuffer();

    switch (filterOption)
    {
    case 1:
      printf("\nQual a prioridade? (1-Baixa, 2-Media, 3-Alta, 4-Critica): ");
      if (scanf("%d", &filterValue) == 1)
      {
        listTicketsByPriority(filterValue);
      }
      else
      {
        puts("Entrada inválida!");
      }
      clearBuffer();
      waitForKey();
      break;

    case 2:
    {

      int realId;
      int displayId;
      puts("\n== ESCOLHA O TIPO ==\n");
      listTicketTypes();
      printf("\nOpção: ");
      scanf("%d", &displayId);
      clearBuffer();

      realId = getRealTypeId(displayId);
      listTicketsByType(realId);
      waitForKey();
      break;
    }
    case 3:
      printf("\nQual o estado? (1-Aberto, 2-Em Atend., 3-Esp. User, "
             "4-Resolvido, 5-Fechado): ");
      if (scanf("%d", &filterValue) == 1)
      {
        listTicketsByStatus(filterValue);
      }
      else
      {
        puts("Entrada inválida!");
      }
      clearBuffer();
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

void menuAdminOrder()
{

  int orderOption;
  int running;

  do
  {
    puts("\n== ORDENAR ==");
    puts("1  - Ordenar tickets por data");
    puts("2  - Ordenar tickets por prioridade");
    puts("3  - Ordenar tickets por tecnico");
    puts("0  - Voltar");
    printf("Opcao: ");
    scanf("%d", &orderOption);
    clearBuffer();
    switch (orderOption)
    {

    case 1:
      sortTicketsByDate();
      listAllTickets();
      waitForKey();
      break;
    case 2:
      sortTicketsByPriority();
      listAllTickets();
      waitForKey();
      break;
    case 3:
      sortTicketsByTechnician();
      listAllTickets();
      waitForKey();
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

void menuManagmentTypes()
{
  int orderOption;
  int running = 0;

  do
  {
    puts("\n== GERIR CATEGORIAS ==");
    puts("1  - Listar");
    puts("2  - Adicionar");
    puts("3  - Editar");
    puts("4  - Remover");
    puts("0  - Voltar");
    printf("Opcao: ");
    scanf("%d", &orderOption);
    clearBuffer();
    switch (orderOption)
    {

    case 1:
      puts("=== LISTAR TIPOS ===");
      listTicketTypes();
      break;
    case 2:
      TICKET_TYPE ticketType;
      puts("Insira o nome do novo tipo");
      fgets(ticketType.name, sizeof(ticketType.name), stdin);
      ticketType.name[strcspn(ticketType.name, "\n")] = 0;
      createTicketType(ticketType);
      waitForKey();
      break;
    case 3:
      int idToUpdate;
      puts("Introduza o #ID do ticket que deseja editar");
      scanf("%d", &idToUpdate);
      clearBuffer();
      updateTicketType(idToUpdate);
      waitForKey();
      break;
    case 4:
    {
      int idToDelete;
      puts("Introduza o #ID do ticket que deseja editar");
      scanf("%d", &idToDelete);
      clearBuffer();
      deleteType(idToDelete);
      waitForKey();
      break;
    }
    // case ....
    case 0:
      running = 1;
      break;
    default:
      puts("Opção inválida!");
      break;
    }

  } while (!running);
}

void adminMenu()
{
  int option;
  do
  {
    system("cls");
    navbar();
    puts("======== MENU ADMINISTRADOR ========");
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
    puts("15 - Gerar relatorio semanal/mensal");
    puts("16 - Alertas de tickets fora do SLA");
    puts("0  - Logout");
    puts("====================================");
    printf("Opcao: ");
    scanf("%d", &option);
    clearBuffer();

    switch (option)
    {
    case 1:
    {
      TICKET_INFO ticket;
      int realId, displayId;
      printf("**ADICIONAR TICKET**\n");
      do
      {
        puts("Tipos  disponivéis:");
        listTicketTypes();
        printf("Opção: ");
        scanf("%d", &displayId);

        realId = getRealTypeId(displayId);
        printf("\n%dRealid", realId);
        if (realId == -1)
        {
          puts("Opção inválida");
        }
        else
        {
          ticket.typeId = realId;
          break;
        }
      } while (1);

      clearBuffer();

      printf("Descricao: ");
      fgets(ticket.description, 500, stdin);
      ticket.description[strcspn(ticket.description, "\n")] = 0;

      printf("Prioridade (1-Baixa, 2-Media, 3-Alta, 4-Critica): ");
      scanf("%d", &ticket.priority);
      clearBuffer();

      printf("Utilizador que reportou: ");
      fgets(ticket.user, MAX_STR, stdin);
      ticket.user[strcspn(ticket.user, "\n")] = 0;

      createTicket(ticket);
      waitForKey();
      break;
    }
    case 2:
    {
      int ticketId;
      printf("Introduza o #ID do ticket que deseja editar: ");
      scanf("%d", &ticketId);
      clearBuffer();
      updateTicket(ticketId);
      waitForKey();
      break;
    }
    case 3:
    {

      system("cls");
      listAllTickets();
      waitForKey();
      break;
    }
    case 4:
    {
      int ticketId = 0;
      puts("Digita o id do ticket a visualizar");
      scanf("%i", &ticketId);
      clearBuffer();
      showTicketById(ticketId);
      waitForKey();
      break;
    }
    case 5:
    {
      int ticketId;
      printf("Introduza o ticket que deseja remover: ");
      scanf("%d", &ticketId);
      clearBuffer();
      deleteTicket(ticketId);
      waitForKey();
      break;
    }
    case 6: // Apresentar Filtrado
      menuAdminFilter();
      break;
    case 7: // Apresentar Ordenado
      menuAdminOrder();
      break;
    case 8: /* gerirCategorias() */
      menuManagmentTypes();
      break;
    case 9:
      listAllTechnicians();
      waitForKey();
      break;
    case 10:
    {
      listPendingTechnicians();

      int userId;
      printf("ID do tecnico a validar (0 para cancelar): ");
      scanf("%d", &userId);
      clearBuffer();

      if (userId != 0)
      {
        validateTechnician(userId);
      }
      waitForKey();
      break;
    }
    case 11:
    {
      int ticketId, tecnicoId;

      printf("**ATRIBUIR TECNICO A TICKET**\n");
      printf("ID do ticket: ");
      scanf("%d", &ticketId);
      clearBuffer();

      printf("ID do tecnico: ");
      scanf("%d", &tecnicoId);
      clearBuffer();

      assignTechnician(ticketId, tecnicoId);
      waitForKey();
      break;
    }
    case 12:
    {
      int id;
      printf("ID do ticket: ");
      scanf("%d", &id);
      clearBuffer();
      printTicketHistory(id);
      waitForKey();
      break;
    }
    case 13: /* tempoMedioPorTecnico() */
      break;
    case 14: /* tempoMedioPorCategoria() */
      break;
    case 15:
    {
      int optionReport;
      int isRunning = 0;

      do
      {
        puts("Tipo de relatório:");
        puts("1 - Mensal");
        puts("2 - Semanal");

        printf("Opcao: ");

        scanf("%d", &optionReport);

        clearBuffer();

        switch (optionReport)
        {
        case 1:
        {
          int month;
          int year;

          printf("Introduza o mes e ano (mm/YYYY): ");

          if (scanf("%d/%d", &month, &year) != 2)
          {
            clearBuffer();
            puts("Formato invalido.");
            break;
          }

          clearBuffer();
          generateMonthReport(month, year);
          isRunning = 1;
          waitForKey();

          break;
        }
        case 2:
        {
          puts("Relatorio semanal ainda nao implementado.");
          isRunning = 1;

          break;
        }
        default:
        {
          puts("Opcao invalida.");
          break;
        }
        }

      } while (!isRunning);

      break;
    }
    case 16: /* alertasSLA() */
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

void technicianMenu(char *username, int logged_userId)
{
  int option;
  do
  {
    system("cls");
    printf("Utilizador: %s\n\n", username);
    printf("======== MENU TECNICO ========\n");
    printf("1 - Ver os meus tickets\n");
    printf("2 - Aceitar ticket pendente\n");
    printf("3 - Atualizar estado de um ticket\n");
    printf("4 - Adicionar comentario/acao\n");
    printf("5 - Delegar ticket a outro tecnico\n");
    printf("6 - Registar ferramentas usadas\n");
    printf("7 - Exportar tickets para CSV\n");
    printf("0 - Logout\n");
    printf("==============================\n");
    printf("Opcao: ");
    scanf("%d", &option);
    clearBuffer();

    switch (option)
    {
    case 1:
      showTicketByTechnician(logged_userId);
      waitForKey();
      break;
    case 2:
      break;
    case 3: /* atualizarEstadoTicket(username) */
    {
      int ticketId;
      printf("\nIntroduza o ID do ticket que deseja atualizar o estado: (0 para cancelar): ");
      scanf("%d", &ticketId);
      clearBuffer();

      if (ticketId == 0)
      {
        waitForKey();
        break;
      }

      updateTicketStatus(ticketId, logged_userId);
      waitForKey();
      break;
    }
    case 4: /* adicionarComentario(username) */
      break;
    case 5: /* delegarTicket(username) */
      break;
    case 6: /* registarFerramentas(username) */
      break;
    case 7: /* exportarCSV(username) */
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

int main()
{
  system("chcp 65001 > nul");
  system("cls");
  char username[MAX_STR], pass[MAX_STR], newPassword[MAX_STR];
  int option;
  int isLogged = 0;
  USER_INFO user;

  createAdmin();

  seederTypes();
  seederTickets();
  while (1)
  {
    system("cls");
    printf("======== SISTEMA DE HELPDESK ========\n");

    printf("\n1 - Login\n");
    printf("2 - Registar\n");
    printf("3 - Alterar PW\n");
    printf("0 - Sair\n");
    printf("Opcao: ");
    scanf("%d", &option);
    clearBuffer();
    system("cls");

    if (option == 0)
      break;

    switch (option)
    {
    case 1:
      printf("**LOGIN**\n");
      printf("Username: ");
      fgets(username, MAX_STR, stdin);
      username[strcspn(username, "\n")] = 0;

      printf("Password: ");
      fgets(pass, MAX_STR, stdin);
      pass[strcspn(pass, "\n")] = 0;
      int logged_userId = -1;

      int perfil = login(username, pass, &logged_userId);

      if (perfil == -1)
      {
        printf("\nCredenciais invalidas!\n");
        waitForKey();
      }
      else if (perfil == -2)
      {
        puts("\nTécnico ainda não validado! Contacte o administrador de sistema\n");
        waitForKey();
      }
      else if (perfil == PERFIL_ADMIN)
      {
        if (isLogged == 1)
        {
          puts("Utilizador logado com sucesso");
        }
        waitForKey();
        system("cls");

        if (isLogged != 1)
        {
          puts("\n=== Primeiro login: altere a password! ===\n");
          printf("Nova password: ");
          fgets(newPassword, MAX_STR, stdin);
          newPassword[strcspn(newPassword, "\n")] = 0;

          if (changePassword(username, newPassword) == 0)
          {
            puts("Password alterada com sucesso!");
            isLogged = 1;
            puts("\nInicio de sessao automatico...");
            waitForKey();
            system("cls");
          }
          else
          {
            puts("Ocorreu um erro ao alterar palavra-passe");
          }
        }
        adminMenu();
      }
      else if (perfil == PERFIL_TECNICO)
      {
        technicianMenu(username, logged_userId);
      }
      system("cls");
      break;

    case 2:
      printf("**REGISTAR**\n");
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

      if (registerUser(user) == -1)
      {
        printf("ERRO: Registo mal sucedido\n");
        break;
      }
      puts("Utilizador criado com sucesso");
      waitForKey();
      system("cls");
      break;

    case 3:
      printf("**ALTERAR PW**\n");
      printf("Username: ");
      fgets(username, MAX_STR, stdin);
      username[strcspn(username, "\n")] = 0;

      printf("Password: ");
      fgets(newPassword, MAX_STR, stdin);
      newPassword[strcspn(newPassword, "\n")] = 0;

      if (changePassword(username, newPassword) == 0)
      {
        puts("Password alterada com sucesso!");
      }
      else
      {
        puts("Ocorreu um erro ao alterar palavra-passe");
      }
      waitForKey();
      system("cls");
      break;
    }
  }

  return 0;
}