#include "s21_string.h"

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
  unsigned char *pDestination = dest;
  const unsigned char *pSource = src;

  if (pDestination && pSource) {
    if (s21_strlen(src) < s21_strlen(dest)) {
      while (n--) {
        *(pDestination++) = *(pSource++);
      }
    } else {
      s21_memcpy(dest, src, n);
    }
  }
  return dest;
}
