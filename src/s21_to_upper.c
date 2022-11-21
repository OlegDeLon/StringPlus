#include "s21_string.h"

void *s21_to_upper(const char *string) {
  short isError = 1;
  char *pString = calloc((s21_strlen(string) + 1), sizeof(char));

  if (pString) {
    s21_strcpy(pString, string);
    for (char *p = (char *)pString; *p != '\0'; p++) {
      if (*p >= 'a' && *p <= 'z') {
        *p -= 32;
      }
    }
    isError = 0;
  }
  return (isError) ? S21_NULL : (void *)pString;
}
