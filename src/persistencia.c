#include "persistencia.h"
#include "funcoes.h"
#include <stdio.h>

int loadAllData(void) {
  int resultCategories;
  int resultUsers;
  int resultTickets;

  puts("\n=== A carregar dados persistidos ===");

  resultCategories = loadCategoryTypesFromFile("data/categories.dat");
  resultUsers = loadUsersFromFile("data/users.dat");
  resultTickets = loadTicketsFromFile("data/tickets.dat");

  if (resultCategories != 0 || resultUsers != 0 || resultTickets != 0) {
    puts("\nAVISO: Nem todos os dados foram carregados com sucesso.\n");
    waitForKey();
    return -1;
  }

  return 0;
}

int backupAllData(void) {
  int resultUsers;
  int resultTickets;
  int resultCategories;

  printf("\n=== A guardar dados ===\n");

  resultUsers = saveUsersToFile("data/users.dat");
  resultTickets = saveTicketsToFile("data/tickets.dat");
  resultCategories = saveCategoryTypesToFile("data/categories.dat");

  // Basta 1 ser diferente de 0 (sucesso) para lancar este erro
  if (resultUsers != 0 || resultTickets != 0 || resultCategories != 0) {
    waitForKey();
    puts("\nERRO: Nem todos os dados foram guardados com sucesso.\n");
    return -1;
  }
  return 0;
}
