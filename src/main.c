#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>

void menuAdministrador()
{
  int opcao;
  do
  {
    system("cls");
    puts("======== MENU ADMINISTRADOR ========");
    puts("1  - Adicionar ticket");
    puts("2  - Listar todos os tickets");
    puts("3  - Filtrar tickets por estado");
    puts("4  - Filtrar tickets por prioridade");
    puts("5  - Filtrar tickets por tipo");
    puts("6  - Ordenar tickets por data");
    puts("7  - Ordenar tickets por prioridade");
    puts("8  - Ordenar tickets por tecnico");
    puts("9  - Remover ticket");
    puts("10 - Editar ticket");
    puts("11 - Adicionar/Remover/Editar categorias");
    puts("12 - Validar tecnicos pendentes");
    puts("13 - Ver historico de um ticket");
    puts("14 - Tempo medio de resolucao por tecnico");
    puts("15 - Tempo medio de resolucao por categoria");
    puts("16 - Gerar relatorio semanal/mensal");
    puts("17 - Alertas de tickets fora do SLA");
    puts("0  - Logout");
    puts("====================================");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparbuffer();

    switch (opcao)
    {
      // precisa de "{}" por causa da declaraçao da variavel
    case 1:
    {
      INFO_TICKET ticket;

      printf("**ADICIONAR TICKET**\n");
      do
      {
        printf("Tipo (1-Hardware, 2-Software, 3-Rede, 4-Acesso, 5-Outro): ");

        if (scanf("%d", &ticket.tipo) != 1 || (ticket.tipo < TIPO_HARDWARE || ticket.tipo > TIPO_OUTRO)) // Se o retorno for diferente de 1 é porque não leu um inteiro.
        {
          puts("Tipo de TICKET não válido.");
          limparbuffer();
        }
        else
        {
          break; // Input válido, sai do ciclo
        }
      } while (1);

      limparbuffer();

      printf("Descricao: ");
      fgets(ticket.descricao, 500, stdin);
      ticket.descricao[strcspn(ticket.descricao, "\n")] = 0;

      printf("Prioridade (1-Baixa, 2-Media, 3-Alta, 4-Critica): ");
      scanf("%d", &ticket.prioridade);
      limparbuffer();

      printf("Utilizador que reportou: ");
      fgets(ticket.utilizador, MAX_STR, stdin);
      ticket.utilizador[strcspn(ticket.utilizador, "\n")] = 0;

      ticket.abertura = obterDataAtual();

      criarTicket(ticket);
      esperarTecla();
      break;
    }
    case 2: /* editarTicket() */
      editarTicket();
      break;
    case 3:
      listarTickets();
      esperarTecla();
      break;
    case 4:
      // verTicketPorID(); comentei devido ao erro dado
      esperarTecla();
    case 5: /* removerTicket() */
      int idToRemove;
      printf("Introduza o ticket que deseja remover: ");
      scanf("%d", &idToRemove);
      removerTicket(idToRemove);
      break;
    case 6: /* ordenarPorData() */
      break;
    case 7: /* ordenarPorPrioridade() */
      break;
    case 8: /* ordenarPorTecnico() */
      break;
    case 9: /* filtrarPorPrioridade() */
      break;
    case 10: /* filtrarPorTipo() */
      break;
    case 11: /* filtrarPorEstado() */
      break;
    case 12: /* gerirCategorias() */
      break;
    case 13: /* validarTecnicos() */
      break;
    case 14: /* verHistorico() */
      break;
    case 15: /* tempoMedioPorTecnico() */
      break;
    case 16: /* tempoMedioPorCategoria() */
      break;
    case 17: /* gerarRelatorio() */
      break;
    case 18: /* alertasSLA() */
      break;
    case 0:
      printf("Logout...\n");
      break;
    default:
      printf("Opcao invalida!\n");
      esperarTecla();
    }
  } while (opcao != 0);
}

void menuTecnico(char *username)
{
  int opcao;
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
    scanf("%d", &opcao);
    limparbuffer();

    switch (opcao)
    {
    case 1: /* verMeusTickets(username) */
      break;
    case 2: /* aceitarTicket(username) */
      break;
    case 3: /* atualizarEstadoTicket(username) */
      break;
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
      esperarTecla();
    }
  } while (opcao != 0);
}

int main()
{
  system("cls");
  char username[MAX_STR], pass[MAX_STR], newPassword[MAX_STR];
  int opcao;
  int isLogged = 0;
  INFO_UTILIZADOR utilizador;

  criarAdmin();

  while (1)
  {
    printf("======== SISTEMA DE HELPDESK ========\n");

    printf("\n1 - Login\n");
    printf("2 - Registar\n");
    printf("3 - Alterar PW\n");
    printf("0 - Sair\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparbuffer();
    system("cls");

    if (opcao == 0)
      break;

    switch (opcao)
    {
    case 1:
      printf("**LOGIN**\n");
      printf("Username: ");
      fgets(username, MAX_STR, stdin);
      username[strcspn(username, "\n")] = 0;

      printf("Password: ");
      fgets(pass, MAX_STR, stdin);
      pass[strcspn(pass, "\n")] = 0;

      int perfil = login(username, pass);

      if (perfil == -1)
      {
        printf("\nCredenciais invalidas!\n");
        esperarTecla();
      }
      else if (perfil == PERFIL_ADMIN)
      {
        if (isLogged == 1)
        {
          puts("Utilizador logado com sucesso");
        }
        esperarTecla();
        system("cls");

        if (isLogged != 1)
        {
          puts("\n=== Primeiro login: altere a password! ===\n");
          printf("Nova password: ");
          fgets(newPassword, MAX_STR, stdin);
          newPassword[strcspn(newPassword, "\n")] = 0;

          if (alterarPassword(username, newPassword) == 0)
          {
            puts("Password alterada com sucesso!");
            isLogged = 1;
            puts("\nInicio de sessao automatico...");
            esperarTecla();
            system("cls");
          }
          else
          {
            puts("Ocorreu um erro ao alterar palavra-passe");
          }
        }
        menuAdministrador();
      }
      else if (perfil == PERFIL_TECNICO)
      {
        menuTecnico(username);
      }
      system("cls");
      break;

    case 2:
      printf("**REGISTAR**\n");
      printf("Nome: ");
      fgets(utilizador.nome, MAX_STR, stdin);
      utilizador.nome[strcspn(utilizador.nome, "\n")] = 0;

      printf("Username: ");
      fgets(utilizador.username, MAX_STR, stdin);
      utilizador.username[strcspn(utilizador.username, "\n")] = 0;

      printf("Password: ");
      fgets(utilizador.password, MAX_STR, stdin);
      utilizador.password[strcspn(utilizador.password, "\n")] = 0;

      utilizador.perfil = PERFIL_TECNICO;

      if (registarUtilizador(utilizador) == -1)
      {
        printf("ERRO: Registo mal sucedido\n");
      }
      puts("Utilizador criado com sucesso");
      esperarTecla();
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

      if (alterarPassword(username, newPassword) == 0)
      {
        puts("Password alterada com sucesso!");
      }
      else
      {
        puts("Ocorreu um erro ao alterar palavra-passe");
      }
      esperarTecla();
      system("cls");
      break;
    }
  }

  return 0;
}