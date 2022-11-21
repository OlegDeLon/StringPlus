#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *pDestination = dest;
  const char *pSource = src;
  while (n) {
    *(pDestination++) = *(pSource++);
    n--;
  }
  return dest;
}
