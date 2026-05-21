#include "log.h"
#include <stdio.h>
#include <time.h>

#define LOG_FILE "data/audit.log"

void logEvent(const char *user, const char *action, const char *details) {
  FILE *fp = fopen(LOG_FILE, "a"); // append
  if (fp == NULL)
    return;

  DateTime now = getCurrentDateTime();

  fprintf(fp, "[%02d/%02d/%04d %02d:%02d] %s | %s | %s\n", now.day, now.month,
          now.year, now.hour, now.min, user, action, details);

  fclose(fp);
}

void logError(const char *user, const char *action, const char *details) {
  FILE *fp = fopen(LOG_FILE, "a");
  if (fp == NULL)
    return;

  DateTime now = getCurrentDateTime();

  fprintf(fp, "[%02d/%02d/%04d %02d:%02d] ERRO | %s | %s | %s\n", now.day,
          now.month, now.year, now.hour, now.min, user, action, details);

  fclose(fp);
}

void logSuccess(const char *user, const char *action, const char *details) {
  FILE *fp = fopen(LOG_FILE, "a");
  if (fp == NULL)
    return;

  DateTime now = getCurrentDateTime();

  fprintf(fp, "[%02d/%02d/%04d %02d:%02d] SUCESSO | %s | %s | %s\n", now.day,
          now.month, now.year, now.hour, now.min, user, action, details);

  fclose(fp);
}