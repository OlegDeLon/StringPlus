#include "s21_string.h"

int s21_strcmp(const char *str1, const char *str2) {
  int result = 0;

  while ((unsigned char)*str1 == (unsigned char)*str2 && *str1 != 0) {
    str1++;
    str2++;
  }
  if ((unsigned char)*str1 != (unsigned char)*str2) {
    result = ((unsigned char)*str1) - ((unsigned char)*str2);
  }
  return result;
}
