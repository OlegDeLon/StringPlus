#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  char *pTemp = (char *)str;
  char symbol = c;
  char *pEntry = S21_NULL;
  for (s21_size_t i = 0; i < n; i++) {
    if (pTemp[i] == symbol) {
      pEntry = &pTemp[i];
      break;
    }
  }
  return (void *)pEntry;
}
