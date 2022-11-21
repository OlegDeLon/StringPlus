#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  unsigned char *pString1 = (unsigned char*)str1;
  unsigned char *pString2 = (unsigned char*)str2;
  s21_size_t i = 0;
  int result = 0;

  while ((pString1[i] != '\0' || pString2[i] != '\0') && i < n) {
    if (pString1[i] != pString2[i]) {
      result = pString1[i] - pString2[i];
      break;
    }
    i++;
  }
  return result;
}
