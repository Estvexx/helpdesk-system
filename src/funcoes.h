#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 100

#define TIPO_HARDWARE 1
#define TIPO_SOFTWARE 2
#define TIPO_REDE 3
#define TIPO_ACESSO 4
#define TIPO_OUTRO 5

#define ESTADO_ABERTO 1
#define ESTADO_EM_ATENDIMENTO 2
#define ESTADO_ESPERA_UTILIZADOR 3
#define ESTADO_RESOLVIDO 4
#define ESTADO_FECHADO 5

#define PERFIL_ADMIN 1
#define PERFIL_TECNICO 2

typedef struct data {
    int dia, mes, ano;
    int hora, min;
} DataHora;

typedef struct utilizador {
    int id;
    char nome[MAX_STR];
    char username[MAX_STR];
    char password[MAX_STR];
    int perfil;        // ADMIN ou TECNICO
    int validado;      // 0 = pendente, 1 = validado
} INFO_UTILIZADOR;

typedef struct elemUtilizador {
    INFO_UTILIZADOR info;
    struct elemUtilizador *next;
} ELEMENTO_UTILIZADOR;

typedef struct historico{
    DataHora data;
    char utilizador[MAX_STR];
    char descricao[300];
    char tipo_acao[30];
    char tecnico_anterior[30];
    char tecnico_atual[30];
    char estado_anterior[30];
    char estado_atual[30];  
} INFO_HISTORICO;

typedef struct elemHistorico {
    INFO_HISTORICO dados;
    struct elemHistorico *next;
} ELEMENTO_HISTORICO;

typedef struct ticket {
    int id;
    int tipo;          // hardware, software, rede, acesso, outro
    DataHora abertura;
    DataHora fecho;
    char descricao[500];
    int prioridade;    // 1 = baixa, 2 = media, 3 = alta, 4 = critica
    int estado;        // aberto, em atendimento, etc.
    char utilizador[MAX_STR];   // quem reportou
    int tecnico_id;    // tecnico responsavel (-1 se nenhum)
    char solucao[500];
} INFO_TICKET;

typedef struct elemTicket {
    INFO_TICKET dados;
    ELEMENTO_HISTORICO *historico;
    struct elemTicket *next;
} ELEMENTO_TICKET;

DataHora obterDataAtual();

// Retorna -1 se der erro e 0 sucesso
//             GERAL : -1 ERRO -> 0 SUCESSO
// ======================= PARTE INICIAL USERS =======================

int criarAdmin(); // Feito
int registarUtilizador(INFO_UTILIZADOR novo); // Feito
int existeUserbyUsername(char *username); // Feito
int quantidadeUsers(); // Feito
int alterarPassword(char *username, char *newPassword); //Feito
int login(char *username, char *password);  // Feito

// ======================= PARTE TICKETS =======================
int criarTicket(INFO_TICKET ticket); // Feito
int editarTicket(INFO_TICKET ticket);
int removerTicket(int id);
void listarTickets(); // feito
void verTicketPorID(); // feito
int quantidadeTickets(); // feito
int atribuirTecnico(int ticket_id, int tecnico_id);
int atualizarEstado(int ticket_id, int novo_estado);

