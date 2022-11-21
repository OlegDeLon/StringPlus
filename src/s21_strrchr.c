#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  char *pResult = S21_NULL;
  if (c) {
    for (int i = 0; str[i] != '\0'; i++) {
      if (str[i] == (char)c) {
        pResult = (char *)&str[i];
      }
    }
  } else {
    pResult = (char *)str + s21_strlen(str);
  }
  return pResult;
}
