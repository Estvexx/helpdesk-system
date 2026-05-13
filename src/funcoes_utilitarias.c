#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>

void limparbuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void esperarTecla()
{
    puts("\nCarregue numa tecla para continuar ...");
    getchar();
}