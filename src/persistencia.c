#include "persistencia.h"
#include "funcoes.h"
#include <stdio.h>

int loadAllData(void) {
  puts("\n=== Carregando dados persistidos ===");

  loadCategoryTypesFromFile("data/categories.dat");
  loadUsersFromFile("data/users.dat");
  loadTicketsFromFile("data/tickets.dat");

  puts("");
  return 0;
}

int backupAllData(void) {
  printf("\n=== Guardando dados ===\n");

  saveUsersToFile("data/users.dat");
  saveTicketsToFile("data/tickets.dat");
  saveCategoryTypesToFile("data/categories.dat");

  puts("✓ Dados guardados com sucesso!\n");
  return 0;
}
