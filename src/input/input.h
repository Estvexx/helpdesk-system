#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int readInt(const char *prompt);
int readIntRange(const char *prompt, int min, int max);
void readString(const char *prompt, char *buffer, int maxLen);
int validateStringLength(char *str, size_t numMin, size_t numMax);

#endif