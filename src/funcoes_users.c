#include "funcoes.h"
#include <stdio.h>
#include <string.h>

ELEM_USER *head = NULL;

// ======================= PARTE USERS =======================

int createAdmin()
{
  ELEM_USER *new = malloc(sizeof(ELEM_USER));
  if (new == NULL)
  {
    puts("Erro ao alocar a memóra");
    return -1;
  }
  new->info.id = 1;
  strcpy(new->info.name, "Administrador");
  strcpy(new->info.username, "admin");
  strcpy(new->info.password, "admin");
  new->info.perfil = PERFIL_ADMIN;
  new->info.isValidated = 1;
  new->next = head;
  head = new;
  return 0;
}

int registerUser(USER_INFO newUser)
{
  if (userExistsByUsername(newUser.username) == 0)
  {
    puts("Username ja existe!");
    return -1;
  }

  ELEM_USER *new = malloc(sizeof(ELEM_USER));
  if (new == NULL)
  {
    puts("Erro ao alocar a memória");
    return -1;
  }

  new->info = newUser;
  new->info.id = getUserCount() + 1;

  new->info.isValidated = 0;

  new->next = head;
  head = new;

  puts("Registado com sucesso!\n");
  return 0;
}

// Retorna 0 se encontra e -1 senão
int userExistsByUsername(char *username)
{
  ELEM_USER *temp = head;
  while (temp != NULL)
  {
    if (strcmp(temp->info.username, username) == 0)
    {
      return 0;
    }
    temp = temp->next;
  }
  return -1;
}

int getUserCount()
{
  ELEM_USER *temp = head;
  int count = 0;

  while (temp != NULL)
  {
    count++;
    temp = temp->next;
  }
  return count;
}

int login(char *username, char *password)
{
  if (userExistsByUsername(username) != 0)
  {
    return -1;
  }

  ELEM_USER *temp = head;
  while (temp != NULL)
  {
    if (strcmp(temp->info.username, username) == 0)
    {
      if (strcmp(temp->info.password, password) == 0)
      {
        return temp->info.perfil;
      }
      else
      {
        return -1;
      }
    }
    temp = temp->next;
  }
  return -1;
}

int changePassword(char *username, char *newPassword)
{
  ELEM_USER *temp = head;
  while (temp != NULL)
  {
    if (strcmp(temp->info.username, username) == 0)
    {
      strcpy(temp->info.password, newPassword);
      return 0;
    }
    temp = temp->next;
  }

  return -1;
}

// ======================= PARTE TICKETS =======================
