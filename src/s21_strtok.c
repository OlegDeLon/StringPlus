#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *pLast = S21_NULL;
  char *pResult = S21_NULL;
  short isError = 1;

  if (str) {
    pLast = str;
  }

  if (pLast && *pLast) {
    char *c = pLast;
    while (s21_strchr(delim, *c)) {
      c++;
    }
    if (*c) {
      char *start = c;
      while (*c && !s21_strchr(delim, *c)) {
        c++;
      }
      if (*c == 0) {
        pLast = c;
        pResult = start;
        isError = 0;
      } else {
        *c = 0;
        pLast = c + 1;
        pResult = start;
        isError = 0;
      }
    }
  }
  return (isError) ? S21_NULL : pResult;
}
