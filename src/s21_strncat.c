#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *pResult = dest + s21_strlen(dest);
  while (n--) {
    if ((*pResult++ = *src++) == '\0') {
      break;
    }
  }
  *pResult = '\0';
  return dest;
}
