#include "s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *pString = str;

  for (; 0 != n; n--) {
    *(pString++) = (unsigned char)c;
  }
  return str;
}
