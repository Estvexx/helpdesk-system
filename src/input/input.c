#include "input.h"

static void clearBuffer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

// Lê um inteiro simples
int readInt(const char *prompt) {
  int num;
  printf("%s", prompt);
  while (scanf("%d", &num) != 1) {
    clearBuffer();
    printf("\nValor inválido!\n %s", prompt);
  }
  clearBuffer();
  return num;
}

// Lê um inteiro dentro de um intervalo
int readIntRange(const char *prompt, int min, int max) {
  int num;
  do {
    num = readInt(prompt);
    if (num < min || num > max) {
      printf("\nOpção inválida! (%d-%d)\n", min, max);
    }
  } while (num < min || num > max);
  return num;
}

// Lê um inteiro e aplica filtro personalizado
int readIntFilter(const char *prompt, const char *errorMsg) {
  int num;
  printf("%s", prompt);
  while (scanf("%d", &num) != 1) {
    clearBuffer();
    printf("%s\n%s", errorMsg, prompt);
  }
  clearBuffer();
  return num;
}

// ===== LEITURA DE STRINGS =====

// Lê uma string genérica
void readString(const char *prompt, char *buffer, int maxLen) {
  printf("%s", prompt);
  fgets(buffer, maxLen, stdin);
  buffer[strcspn(buffer, "\n")] = 0;
}

int charIsValid(char *str, size_t numMin, size_t numMax) {
  size_t lenght = strlen(str);

  if (lenght < numMin) {
    printf("\nInvalido, minimo %zu caracteres.\n", numMin);
    return -1;
  } else if (lenght > numMax) {
    printf("\nInvalido, maximo %zu caracteres.\n", numMax);
    return -1;
  } else {
    return 0;
  }
}
