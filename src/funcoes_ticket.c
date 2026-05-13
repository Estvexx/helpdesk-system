#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void obterTipo(int tipo, char *texto) {
  switch (tipo) {
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

void obterEstado(int estado, char *texto) {
  switch (estado) {
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

ELEMENTO_TICKET *headTickets = NULL;

DataHora obterDataAtual() {
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

int criarTicket(INFO_TICKET ticket) {
  ELEMENTO_TICKET *new = malloc(sizeof(ELEMENTO_TICKET));
  if (new == NULL) {
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

void listarTickets() {
  if (headTickets == NULL) {
    puts("Nenhum ticket registado.");
    return;
  }

  ELEMENTO_TICKET *temp = headTickets;

  printf("\n%-5s | %-9s | %-12s | %-10s | %-15s | %-10s\n", "ID", "Tipo",
         "Estado", "Prioridade", "Utilizador", "Tecnico");
  printf("------+-----------+--------------+------------+-----------------+----"
         "--------\n");

  while (temp != NULL) {
    char tipo[15], estado[20], prioridade[10];

    obterTipo(temp->dados.tipo, tipo);
    obterEstado(temp->dados.estado, estado);
    obterPrioridade(temp->dados.prioridade, prioridade);

    if (temp->dados.tecnico_id == -1) {
      printf("%-5d | %-9s | %-12s | %-10s | %-15s | %-10s\n", temp->dados.id,
             tipo, estado, prioridade, temp->dados.utilizador, "N/D");
    } else {
      printf("%-5d | %-9s | %-12s | %-10s | %-15s | %-10d\n", temp->dados.id,
             tipo, estado, prioridade, temp->dados.utilizador,
             temp->dados.tecnico_id);
    }

    temp = temp->next;
  }
  printf("\n");
}

int quantidadeTickets() {
  ELEMENTO_TICKET *temp = headTickets;
  int count = 0;
  while (temp != NULL) {
    count++;
    temp = temp->next;
  }
  return count;
}

void verTicketPorID(int id) {
  ELEMENTO_TICKET *temp = headTickets;

  while (temp != NULL) {
    if (temp->dados.id == id) {
      char tipo[15], estado[20], prioridade[10];

      obterTipo(temp->dados.tipo, tipo);
      obterEstado(temp->dados.estado, estado);
      obterPrioridade(temp->dados.prioridade, prioridade);

      printf("========================================\n");
      printf("Ticket #%d\n", temp->dados.id);
      printf("========================================\n");
      printf("Tipo:        %s\n", tipo);
      printf("Estado:      %s\n", estado);
      printf("Prioridade:  %s\n", prioridade);
      printf("Descricao:   %s\n", temp->dados.descricao);
      printf("Utilizador:  %s\n", temp->dados.utilizador);
      printf("Tecnico:     ");
      if (temp->dados.tecnico_id == -1)
        printf("N/D\n");
      else
        printf("%d\n", temp->dados.tecnico_id);
      printf("Abertura:    %02d/%02d/%04d %02d:%02d\n",
             temp->dados.abertura.dia, temp->dados.abertura.mes,
             temp->dados.abertura.ano, temp->dados.abertura.hora,
             temp->dados.abertura.min);
      printf("Solucao:     %s\n", strcmp(temp->dados.solucao, "") == 0
                                      ? "N/D"
                                      : temp->dados.solucao);
      printf("\n");
      return;
    }
    temp = temp->next;
  }
  puts("Ticket nao encontrado.");
}
