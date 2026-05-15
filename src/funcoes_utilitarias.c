#include "funcoes.h"
#include <stdio.h>

void clearBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void waitForKey() {
  puts("\nCarregue numa tecla para continuar ...");
  getchar();
}

// Retorna 1 se d1 > d2, 0 se igual, -1 se d1 < d2
int compareDates(DateTime d1, DateTime d2) {
  if (d1.year > d2.year)
    return 1;
  if (d1.year < d2.year)
    return -1;
  if (d1.month > d2.month)
    return 1;
  if (d1.month < d2.month)
    return -1;
  if (d1.day > d2.day)
    return 1;
  if (d1.day < d2.day)
    return -1;
  if (d1.hour > d2.hour)
    return 1;
  if (d1.hour < d2.hour)
    return -1;
  if (d1.min > d2.min)
    return 1;
  if (d1.min < d2.min)
    return -1;
  return 0;
}