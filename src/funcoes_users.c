#include "funcoes.h"
#include <stdio.h>
#include <string.h>

ELEM_USER *head = NULL;

void cleanupIntermediateUsers() { cleanupUsers(head); };

int nextUserId = 1;

void inicializarIds_Users() {
  // AQUI NAO PRECISO DE VERIFICAR SE A LSITA ESTA VAZIA
  ELEM_USER *temp = head;
  while (temp != NULL) {
    if (temp->info.id > nextUserId) {
      nextUserId = temp->info.id + 1;
    }
    temp = temp->next;
  }
}

// ======================= PARTE USERS =======================

// Return 1 - erro / Return 0 é admin / Return -1 não é
int isAdmin(int logged_userId) {
  ELEM_USER *temp = head;

  while (temp != NULL && temp->info.id != logged_userId) {
    temp = temp->next;
  }

  if (temp == NULL) {
    return 1;
  }

  if (temp->info.perfil == PERFIL_ADMIN) {
    return 0;
  } else {
    return -1;
  }

  return 1;
}

int createAdmin() {
  ELEM_USER *new = malloc(sizeof(ELEM_USER));
  if (new == NULL) {
    puts("\nERRO: Falha ao alocar a memóra");
    return -1;
  }
  new->info.id = nextUserId;
  nextUserId++;
  strcpy(new->info.name, "Administrador");
  strcpy(new->info.username, "admin");
  strcpy(new->info.password, "admin");
  new->info.perfil = PERFIL_ADMIN;
  new->info.isValidated = 0;
  new->next = head;
  head = new;
  return 0;
}

int registerUser(USER_INFO newUser) {
  if (userExistsByUsername(newUser.username) == 0) {
    puts("Username ja existe!");
    return -1;
  }

  ELEM_USER *new = malloc(sizeof(ELEM_USER));
  if (new == NULL) {
    puts("\nERRO: Falha ao alocar a memória");
    return -1;
  }

  new->info = newUser;
  new->info.id = nextUserId;
  nextUserId++;

  new->info.isValidated = 0;

  new->next = head;
  head = new;

  puts("Registado com sucesso!\n");
  return 0;
}

// Retorna 0 se encontra e -1 senão
int userExistsByUsername(char *username) {
  ELEM_USER *temp = head;
  while (temp != NULL) {
    if (strcmp(temp->info.username, username) == 0) {
      return 0;
    }
    temp = temp->next;
  }
  return -1;
}

int getUserCount() {
  ELEM_USER *temp = head;
  int count = 0;

  while (temp != NULL) {
    count++;
    temp = temp->next;
  }
  return count;
}

int checkAdminValidated() {
  ELEM_USER *temp = head;
  while (temp != NULL) {
    if (temp->info.perfil == PERFIL_ADMIN && temp->info.isValidated == 1) {
      return 0;
    }
    temp = temp->next;
  }
  return -1;
}

int login(char *username, char *password, int *id) {
  if (userExistsByUsername(username) != 0) {
    return -1;
  }

  ELEM_USER *temp = head;
  while (temp != NULL) {
    if (strcmp(temp->info.username, username) == 0) {
      if (temp->info.isValidated == 0 && temp->info.perfil != PERFIL_ADMIN) {
        return -2;
      }
      if (strcmp(temp->info.password, password) == 0) {
        *id = temp->info.id;
        return temp->info.perfil;
      } else {
        return -1;
      }
    }
    temp = temp->next;
  }
  return -1;
}

int changePassword(char *username, char *newPassword) {
  ELEM_USER *temp = head;
  while (temp != NULL) {
    if (strcmp(temp->info.username, username) == 0) {
      strcpy(temp->info.password, newPassword);
      if (temp->info.perfil == PERFIL_ADMIN) {
        temp->info.isValidated = 1;
      }
      return 0;
    }
    temp = temp->next;
  }

  return -1;
}

int validateTechnician(int userId) {
  ELEM_USER *temp = head;

  while (temp != NULL) {
    if (temp->info.id == userId) {
      if (temp->info.perfil != PERFIL_TECNICO) {
        puts("Este utilizador nao e um tecnico!");
        return -1;
      }

      if (temp->info.isValidated == 1) {
        puts("Este tecnico ja foi validado!");
        return -1;
      }

      // Validar
      temp->info.isValidated = 1;
      printf("Tecnico #%d (%s) validado com sucesso!\n", temp->info.id,
             temp->info.name);
      return 0;
    }
    temp = temp->next;
  }

  puts("Utilizador nao encontrado!");
  return -1;
}

void listAllTechnicians() {
  ELEM_USER *temp = head;
  int encontrou = 0;

  printf("\n%-5s | %-20s | %-15s | %-10s\n", "ID", "Nome", "Username",
         "Validado");
  printf("------+----------------------+-----------------+------------\n");

  while (temp != NULL) {
    if (temp->info.perfil == PERFIL_TECNICO) {
      printf("%-5d | %-20s | %-15s | ", temp->info.id, temp->info.name,
             temp->info.username);

      if (temp->info.isValidated == 1) {
        printf("\x1b[32m%c\x1b[0m\n", 'V');
      } else {
        printf("\x1b[31m%c\x1b[0m\n", 'X');
      }

      encontrou = 1;
    }
    temp = temp->next;
  }

  if (!encontrou) {
    puts("Nenhum tecnico registado.");
  }
  printf("\n");
}

void listPendingTechnicians() {
  ELEM_USER *temp = head;
  int encontrou = 0;

  printf("\n%-5s | %-20s | %-15s | %-10s\n", "ID", "Nome", "Username",
         "Validado");
  printf("------+----------------------+-----------------+--------\n");

  while (temp != NULL) {
    if (temp->info.perfil == PERFIL_TECNICO && temp->info.isValidated == 0) {
      printf("%-5d | %-20s | %-15s | \x1b[31mX\x1b[0m\n", temp->info.id,
             temp->info.name, temp->info.username);
      encontrou = 1;
    }
    temp = temp->next;
  }

  if (!encontrou) {
    puts("Nenhum tecnico pendente de validacao.");
  }
  printf("\n");
}

// Retorna 1 -> validado; retorna 0 -> pendente; retorna -1 utilizador nao
// encontrado
int isTechnicianValidated(int userId) {
  ELEM_USER *temp = head;

  while (temp != NULL) {
    if (temp->info.id == userId) {
      if (temp->info.perfil != PERFIL_TECNICO) {
        return -1;
      }

      if (temp->info.isValidated == 1) {
        return 1; // validado
      } else {
        return 0; // pendente
      }
    }
    temp = temp->next;
  }

  return -1; // utilizador não encontrado
}

int saveUsersToFile(const char *filename) {
  FILE *fp = fopen(filename, "wb");
  if (fp == NULL) {
    printf("\nERRO: Não foi possível guardar utilizadores\n");
    return -1;
  }

  int count = getUserCount();
  fwrite(&count, sizeof(int), 1, fp);

  ELEM_USER *temp = head;
  while (temp != NULL) {
    fwrite(&(temp->info), sizeof(USER_INFO), 1, fp);
    temp = temp->next;
  }

  fclose(fp);
  printf("%d utilizadores guardados em: %s\n", count, filename);
  return 0;
}

int loadUsersFromFile(const char *filename) {
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("Ficheiro %s não encontrado (primeira execução?)\n", filename);
    return -1;
  }

  int count;
  fread(&count, sizeof(int), 1, fp);

  for (int i = 0; i < count; i++) {
    ELEM_USER *new = malloc(sizeof(ELEM_USER));
    if (new == NULL) {
      fclose(fp);
      return -1;
    }

    fread(&(new->info), sizeof(USER_INFO), 1, fp);

    new->next = head;
    head = new;
  }

  fclose(fp);
  printf("%d utilizadores carregados de: %s\n", count, filename);
  return 0;
}
