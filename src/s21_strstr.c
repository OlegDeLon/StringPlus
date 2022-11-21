#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  int k, j;
  char *pString1 = (char *)needle;
  char *pString2 = S21_NULL;
  if (*pString1 != '\0') {
    int flag = 0;
    char *pTemp = (char *)haystack;
    for (int i = 0; pTemp[i] != '\0'; i++) {
      if (flag == 0) {
        if (pTemp[i] == pString1[0]) {
          k = i;
          j = 0;
          for (; pString1[j] != '\0'; j++, k++) {
            if (pTemp[k] != pString1[j]) {
              break;
            }
          }
          if (pString1[j] == '\0') {
            flag = 1;
            pString2 = &pTemp[i];
          }
        }
      } else {
        break;
      }
    }
  } else {
    pString2 = (char *)haystack;
  }
  return pString2;
}
