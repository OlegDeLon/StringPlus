#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *pString1 = str1;
  const unsigned char *pString2 = str2;
  s21_size_t i = 0;
  int result = 0;

  while (i < n) {
    if (pString1[i] != pString2[i]) {
      result = (int)(pString1[i] - pString2[i]);
      break;
    }
    i++;
  }
  return result;
}
