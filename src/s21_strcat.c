#include "s21_string.h"

char *s21_strcat(char *dest, const char *src) {
  char *pResult = dest + s21_strlen(dest);

  while (*src != '\0') {
    *pResult++ = *src++;
  }
  *pResult++ = '\0';
  return dest;
}
