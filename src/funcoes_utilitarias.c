#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>

void clearBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void waitForKey()
{
    puts("\nCarregue numa tecla para continuar ...");
    getchar();
}