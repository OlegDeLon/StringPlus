#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *pTemp1 = (char *)str1;
  char *pTemp2 = (char *)str2;
  char *pEntry = S21_NULL;
  for (int i = 0; pTemp1[i] != '\0'; i++) {
    int f = 0;
    for (int j = 0; pTemp2[j] != '\0'; j++) {
      if (pTemp1[i] == pTemp2[j]) {
        pEntry = &pTemp1[i];
        f = 1;
        break;
      }
    }
    if (f != 0) {
      break;
    }
  }
  return (pEntry);
}
