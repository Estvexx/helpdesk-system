#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void obterTipo(int tipo, char *texto)
{
  switch (tipo)
  {
  case TIPO_HARDWARE:
    strcpy(texto, "Hardware");
    break;
  case TIPO_SOFTWARE:
    strcpy(texto, "Software");
    break;
  case TIPO_REDE:
    strcpy(texto, "Rede");
    break;
  case TIPO_ACESSO:
    strcpy(texto, "Acesso");
    break;
  case TIPO_OUTRO:
    strcpy(texto, "Outro");
    break;
  default:
    strcpy(texto, "???");
    break;
  }
}

void obterEstado(int estado, char *texto)
{
  switch (estado)
  {
  case ESTADO_ABERTO:
    strcpy(texto, "Aberto");
    break;
  case ESTADO_EM_ATENDIMENTO:
    strcpy(texto, "Em Atend.");
    break;
  case ESTADO_ESPERA_UTILIZADOR:
    strcpy(texto, "Esp. User");
    break;
  case ESTADO_RESOLVIDO:
    strcpy(texto, "Resolvido");
    break;
  case ESTADO_FECHADO:
    strcpy(texto, "Fechado");
    break;
  default:
    strcpy(texto, "???");
    break;
  }
}

void obterPrioridade(int prioridade, char *texto)
{
  switch (prioridade)
  {
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

ELEMENTO_TICKET *headTickets = NULL;

DataHora obterDataAtual()
{
  DataHora d;
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  d.dia = tm->tm_mday;
  d.mes = tm->tm_mon + 1;
  d.ano = tm->tm_year + 1900;
  d.hora = tm->tm_hour;
  d.min = tm->tm_min;
  return d;
}

int criarTicket(INFO_TICKET ticket)
{
  ELEMENTO_TICKET *new = malloc(sizeof(ELEMENTO_TICKET));
  if (new == NULL)
  {
    puts("Erro ao alocar memória");
    return -1;
  }

  new->dados = ticket;
  new->dados.id = quantidadeTickets() + 1;
  new->dados.tecnico_id = -1; // deixar -1 porque nao tem tecnico associado
  new->dados.estado = ESTADO_ABERTO;
  strcpy(new->dados.solucao, "");
  new->dados.abertura = obterDataAtual();
  new->dados.fecho.dia = 0;
  new->dados.fecho.mes = 0;
  new->dados.fecho.ano = 0;
  new->dados.fecho.hora = 0;
  new->dados.fecho.min = 0;
  new->historico = NULL;

  new->next = headTickets;
  headTickets = new;

  printf("\nTicket #%d criado com sucesso!\n", new->dados.id);
  return 0;
}

int editarTicket()
{
  int idForSearch;

  printf("Introduza o número do TICKET que deseja editar: ");
  scanf("%d", &idForSearch);

  ELEMENTO_TICKET *temp = headTickets;

  while (temp != NULL)
  {
    if (temp->dados.id == idForSearch)
    {
      puts("=== TICKET ENCONTRADO ===");
      printInfosTicket(temp->dados);

      puts("=== ALTERAR INFORMAÇÕES ===");
      do
      {
        printf("Novo tipo (1-Hardware, 2-Software, 3-Rede, 4-Acesso, 5-Outro): ");

        if (scanf("%d", &temp->dados.tipo) != 1 || (temp->dados.tipo < TIPO_HARDWARE || temp->dados.tipo > TIPO_OUTRO)) // Se o retorno for diferente de 1 é porque não leu 1 inteiro.
        {
          puts("Tipo de TICKET não válido.");
          limparBuffer();
        }
        else
        {
          break; // Input válido, sai do ciclo
        }
      } while (1);

      limparBuffer();

      printf("Descrição: ");
      fgets(temp->dados.descricao, sizeof(temp->dados.descricao), stdin);

      printf("Prioridade (1-Aberto, 2-Em Atend., 3-Esp. User, 4-Resolvido, 5-Fechado): ");
      scanf("%d", &temp->dados.prioridade);

      printf("Estado (1-Baixa, 2-Media, 3-Alta, 4-Critica): ");
      scanf("%d", &temp->dados.estado);

      limparBuffer();

      printf("Tecnico: ");
      // Falta verificação de técnico

      printf("\nTicket editado com sucesso!\n");
      return 0;
    }

    temp = temp->next;
  }
  printf("\nTicket não encontrado!\n");
  return -1;
}

void printInfosTicket(INFO_TICKET ticket)
{
  char tipo[15], estado[20], prioridade[10];

  obterTipo(ticket.tipo, tipo);
  obterEstado(ticket.estado, estado);
  obterPrioridade(ticket.prioridade, prioridade);

  printf("========================================\n");
  printf("Ticket #%d\n", ticket.id);
  printf("========================================\n");
  printf("Tipo:        %s\n", tipo);
  printf("Estado:      %s\n", estado);
  printf("Prioridade:  %s\n", prioridade);
  printf("Descricao:   %s\n", ticket.descricao);
  printf("Utilizador:  %s\n", ticket.utilizador);

  printf("Tecnico:     ");
  if (ticket.tecnico_id == -1)
    printf("N/D\n");
  else
    printf("%d\n", ticket.tecnico_id);

  printf("Abertura:    %02d/%02d/%04d %02d:%02d\n",
         ticket.abertura.dia, ticket.abertura.mes,
         ticket.abertura.ano, ticket.abertura.hora,
         ticket.abertura.min);

  printf("Solucao:     %s\n", strcmp(ticket.solucao, "") == 0 ? "N/D" : ticket.solucao);
  printf("\n");
}

void listarTickets()
{
  if (headTickets == NULL)
  {
    puts("Nenhum ticket registado.");
    return;
  }

  ELEMENTO_TICKET *temp = headTickets;

  printf("\n%-5s | %-9s | %-12s | %-10s | %-15s | %-10s\n", "ID", "Tipo",
         "Estado", "Prioridade", "Utilizador", "Tecnico");
  printf("------+-----------+--------------+------------+-----------------+----"
         "--------\n");

  while (temp != NULL)
  {
    char tipo[15], estado[20], prioridade[10];

    obterTipo(temp->dados.tipo, tipo);
    obterEstado(temp->dados.estado, estado);
    obterPrioridade(temp->dados.prioridade, prioridade);

    if (temp->dados.tecnico_id == -1)
    {
      printf("%-5d | %-9s | %-12s | %-10s | %-15s | %-10s\n", temp->dados.id,
             tipo, estado, prioridade, temp->dados.utilizador, "N/D");
    }
    else
    {
      printf("%-5d | %-9s | %-12s | %-10s | %-15s | %-10d\n", temp->dados.id,
             tipo, estado, prioridade, temp->dados.utilizador,
             temp->dados.tecnico_id);
    }

    temp = temp->next;
  }
  printf("\n");
}

int quantidadeTickets()
{
  ELEMENTO_TICKET *temp = headTickets;
  int count = 0;
  while (temp != NULL)
  {
    count++;
    temp = temp->next;
  }
  return count;
}

void verTicketPorID(int id)
{
  ELEMENTO_TICKET *temp = headTickets;

  while (temp != NULL)
  {
    if (temp->dados.id == id)
    {
      printInfosTicket(temp->dados);
      return;
    }
    temp = temp->next;
  }
  puts("Ticket nao encontrado.");
}

int removerTicket(int id)
{
  int confirmDelete;

  if (headTickets == NULL)
  {
    puts("Nenhum ticket registado para remover.");
    return -1;
  }

  ELEMENTO_TICKET *temp = headTickets;
  ELEMENTO_TICKET *prev = NULL;

  while (temp != NULL && temp->dados.id != id)
  {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL)
  {
    printf("Ticket #%d não encontrado!\n", id);
    return -1;
  }

  printInfosTicket(temp->dados);

  // Confirmação da remoção
  do
  {
    puts("Confirmar remoção?");
    puts("0 - Continuar");
    puts("1 - Cancelar");

    if (scanf("%d", &confirmDelete) != 1 || (confirmDelete != 0 && confirmDelete != 1))
    {
      limparBuffer();
      puts("Insira uma opção válida");
    }
    else
    {
      break;
    }

  } while (1);

  limparBuffer();

  if (confirmDelete == 1)
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
  ELEMENTO_HISTORICO *histTemp = temp->historico;

  while (histTemp != NULL)
  {
    ELEMENTO_HISTORICO *aux = histTemp;
    histTemp = histTemp->next;
    free(aux);
  }

  free(temp);

  printf("Ticket #%d removido com sucesso!\n", id);
  return 0;
}