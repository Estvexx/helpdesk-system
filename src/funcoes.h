#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 100

#define STATUS_OPEN 1          // ESTADO_ABERTO
#define STATUS_WAITING_USER 2 // ESTADO_ESPERA_TECNICO
#define STATUS_IN_PROGRESS 3  // ESTADO_EM_ATENDIMENTO
#define STATUS_RESOLVED 4     // ESTADO_RESOLVIDO
#define STATUS_CLOSED 5       // ESTADO_FECHADO

#define PERFIL_ADMIN 1
#define PERFIL_TECNICO 2

typedef struct datetime
{
    int day, month, year;
    int hour, min;
} DateTime;

typedef struct user
{
    int id;
    char name[MAX_STR];
    char username[MAX_STR];
    char password[MAX_STR];
    int perfil;      // ADMIN ou TECNICO
    int isValidated; // 0 = pendente, 1 = validado
} USER_INFO;

typedef struct elemUser
{
    USER_INFO info;
    struct elemUser *next;
} ELEM_USER;

typedef struct history
{
    DateTime date;
    char user[MAX_STR];
    char description[800];
    char actionType[200];
    char previousTechnician[30];
    char currentTechnician[30];
    char previousStatus[30];
    char currentStatus[30];
} HISTORY_INFO;

typedef struct elemHistory
{
    HISTORY_INFO data;
    struct elemHistory *next;
} ELEM_HISTORY;

typedef struct ticket
{
    int id;
    int typeId;
    DateTime openedAt;
    DateTime closedAt;
    DateTime estimatedConclusion;
    char description[500];
    int priority;       // 1 = baixa, 2 = media, 3 = alta, 4 = critica
    int status;         // aberto, em atendimento, etc.
    char user[MAX_STR]; // quem reportou
    int technicianId;   // tecnico responsavel (-1 se nenhum)
    char actions[300];       // ações realizadas
    char tools[200];        // ferramenta utilizadas
} TICKET_INFO;

typedef struct elemTicket
{
    TICKET_INFO data;
    ELEM_HISTORY *history;
    struct elemTicket *next;
} ELEM_TICKET;

typedef struct ticketType
{
    int id;
    char name[50]; // pre. def: hardware, software, network, access, other
} TICKET_TYPE;

typedef struct elemTicketType
{
    TICKET_TYPE data;
    struct elemTicketType *next;
} ELEM_TICKET_TYPE;

DateTime getCurrentDateTime(); // Obter data atual

//Estrutura para os reports
typedef struct {
    int totalTickets;
    int totalResolved;
    int pendingTickets;
    int countCritical;
    int countHigh;
    int countMedium;
    int countLow;
    long totalMinutesToResolve;
    long maxResolutionTime;
    int maxResolutionTicketId;
    int slaMet;
    int slaViolations;
} ReportStats;


// INICIALIZADOR DE IDS
void inicializarIds_Users();
void inicializarIds_Ticket();
void inicializarIds_TicketType();
// Retorna -1 se der erro e 0 sucesso
//             GERAL : -1 ERRO -> 0 SUCESSO
// ======================= PARTE INICIAL USERS =======================

int createAdmin();                                     // Feito
int registerUser(USER_INFO newUser);                   // Feito
int userExistsByUsername(char *username);              // Feito
int getUserCount();                                    // Feito
int changePassword(char *username, char *newPassword); // Feito
int login(char *username, char *password, int *id);    // Retorn -2 se tecnic não está validado             // Feito
int validateTechnician(int userId);                    // Feito, colocar tecnico como validado
int isTechnicianValidated(int userId);                 // Feito, verificar se tecnico esta validado ou nao
void listPendingTechnicians();                         // Feito
void listAllTechnicians();                             // Feito

// ======================= PARTE TICKETS =======================
int createTicket(TICKET_INFO ticket); // Feito
int updateTicket(int ticketId, int logged_userId );       // Ainda alterações necessárias (preciso tirar duvidas ctg) *preciso alterar logica para perguntar o que ele deseja alterar
int deleteTicket(int ticketId);       // Retorna 1 se for cancelado pelo user
void listAllTickets();                // feito
void listPendentTickts();               // FEITO
int existeUserbyId(int id);           // feito
int showTicketById(int id);          // feito
void showTicketPendentByTechnician(int id); //feito
int getTicketCount();                 // feito
int assignTechnician(int ticket_id, int technicianId);// feito
int updateTicketStatus(int ticket_id, int logged_userId); // Feito
int acceptTicket(int tecnicoId, int ticketId); // Feitos
int delegateTicket(int ticket_id, int logged_userId); // Feito
int exportTicketsCSV(char *username, int userId);

void printInfosTicket(TICKET_INFO ticket);     // Feito
void printInfoFormatTable(TICKET_INFO ticket); // Feito
int addComment(int ticket_id, int logged_userId); // Feito

// Listar Tickets com Filtros
void listTicketsByStatus(int status);     // Feito
void listTicketsByPriority(int priority); // Feito
void listTicketsByType(int type);         // Feito

// Ordenar lista
// PERIGOSO TENHO DE CORRIGIR A ESTRUTURA DO HISTORY
void sortTicketsByTechnician();
void sortTicketsByDate();
void sortTicketsByPriority();
void sortTicketsById();

//RELATÓRIOS
int generateWeeklyReport(DateTime startDate);
int generateMonthReport(int month, int year);
int createPeriodicReports();
//ALERTAS SLA
int alertTicketSLA(int *alertSLA);
void printAlertsSLA();

void averageTimePerTechnician();
void averageTimePerType();
// ======================= PARTE TECNICO =======================
void showTicketByTechnician(int id); // feito

int isAdmin(int logged_userId); // Return 1 - erro / Return 0 é admin / Return -1 não é *FEITO*

// ======================= PARTE HISTORY =======================

void addHistory(ELEM_TICKET *ticket, HISTORY_INFO history);
void printHistory(int ticketId, ELEM_TICKET *headTickets);
// este print é o intermediario que permite utilizar a funçao acima
void printTicketHistory(int ticketId);

// ======================= PARTE TIPOS TICKETS =======================

int createTicketType(TICKET_TYPE ticketType);
int deleteTicketType(int typeId, ELEM_TICKET *headTickets); // Feito
int updateTicketType(int typeId);                           // Feito
void listTicketTypes();                                     // Feito
int getRealTypeId(int displayIndex);                        // Feito
int getTicketTypeCount();                                   // Feito

// ======================= PARTE UTILITARIAS =======================

void getType(int typeId, char *str, ELEM_TICKET_TYPE *headTickets); // Feito
void getStatus(int status, char *str); // Feito
void getPriority(int prioridade, char *texto); // Feito
void clearBuffer(); // Feito
void waitForKey(); // Feito
int validateStringLength(char *str, size_t numMin, size_t numMax);

// Datas                   -------
DateTime addTimeToDateTime(DateTime dt, int hours, int minutes); // Feito
int compareDates(DateTime d1, DateTime d2); // Feito
long differenceInMinutes(DateTime inicio, DateTime fim); // Feito
int daysInMonth(int month, int year); // Feito
//                          --------

int confirmDelete(); // 0 - confirma / 1 - cancela
void tableHeaders(); // Feito

int deleteType(int typeId); // Feito
void headTypes(int typeId, char *str); // Feito
void getTypeUtil(int typeId, char *str); // Feito
int getSLA(int priority);

int createStatsToReports(char *fileName, char *fileHeader, ReportStats stats);


void seederTypes(); // Feito
void seederTickets(); // Feito

// ======================= PARTE EXIT =======================

// Privadas
void cleanupTickets(ELEM_TICKET *headTickets);
void cleanupUsers(ELEM_USER *headUsers);
void cleanupTicketTypes(ELEM_TICKET_TYPE *headTicketsTypes);

// Publicas / INtermediárias
void cleanupIntermediateUsers();
void cleanupIntermediateTickets();
void cleanupIntermediateTicketsTypes();

void cleanAllLists();

// PERSISTENCIA DOS DADOS

int saveUsersToFile(const char *filename);
int loadUsersFromFile(const char *charfilename);

int saveTicketsToFile(const char *filename);
int loadTicketsFromFile(const char *charfilename);

int saveCategoryTypesToFile(const char *filename);
int loadCategoryTypesFromFile(const char *charfilename);

int backupAllData(void);
int loadAllData(void);

int checkAdminValidated();

int validateStringLength(char *str, size_t numMin, size_t numMax);

#endif