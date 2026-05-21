#define INPUT_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int readInt(const char *prompt);
int readIntRange(const char *prompt, int min, int max);
int readIntFilter(const char *prompt, const char *errorMsg);
void readString(const char *prompt, char *buffer, int maxLen);
void readStringAlpha(const char *prompt, char *buffer, int maxLen);
void readStringAlnum(const char *prompt, char *buffer, int maxLen);
int readConfirm(const char *prompt);
