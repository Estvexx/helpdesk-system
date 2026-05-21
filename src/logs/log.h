#ifndef LOG_H
#define LOG_H

#include "../funcoes.h" 

void logEvent(const char *user, const char *action, const char *details);
void logError(const char *user, const char *action, const char *details);
void logSuccess(const char *user, const char *action, const char *details);

#endif