#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  const char *pString1, *pString2;
  s21_size_t count = 0, cmp = 0;
  for (pString1 = str1; *pString1 != '\0'; pString1++) {
    for (pString2 = str2; *pString2 != '\0'; pString2++) {
      if (*pString1 == *pString2) {
        cmp = 1;
        break;
      }
    }
    if (cmp == 1) {
      break;
    }
    count++;
  }
  return count;
}
