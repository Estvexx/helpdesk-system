#include "funcoes.h"
#include <stdio.h>
#include <string.h>

ELEMENTO_UTILIZADOR *head = NULL;

// ======================= PARTE USERS =======================

int criarAdmin()
{
  ELEMENTO_UTILIZADOR *novo = malloc(sizeof(ELEMENTO_UTILIZADOR));
  if (novo == NULL)
  {
    puts("Erro ao alocar a memóra");
    return -1;
  }
  novo->info.id = 1;
  strcpy(novo->info.nome, "Administrador");
  strcpy(novo->info.username, "admin");
  strcpy(novo->info.password, "admin");
  novo->info.perfil = PERFIL_ADMIN;
  novo->info.validado = 1;
  novo->next = head;
  head = novo;
  return 0;
}

int registarUtilizador(INFO_UTILIZADOR info)
{
  if (existeUserbyUsername(info.username) == 1)
  {
    puts("Username ja existe!");
    return -1;
  }

  ELEMENTO_UTILIZADOR *new = malloc(sizeof(ELEMENTO_UTILIZADOR));
  if (new == NULL)
  {
    puts("Erro ao alocar a memória");
    return -1;
  }

  new->info = info;
  new->info.id = quantidadeUsers() + 1;

  new->info.validado = 0;

  new->next = head;
  head = new;

  puts("Registado com sucesso!\n");
  return 0;
}

// Retorna 0 se encontra e -1 senão
int existeUserbyUsername(char *username)
{
  ELEMENTO_UTILIZADOR *temp = head;
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

int quantidadeUsers()
{
  ELEMENTO_UTILIZADOR *temp = head;
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
  if (existeUserbyUsername(username) != 0)
  {
    return -1;
  }

  ELEMENTO_UTILIZADOR *temp = head;
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

int alterarPassword(char *username, char *newPassword)
{
  ELEMENTO_UTILIZADOR *temp = head;
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
