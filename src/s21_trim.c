#include "s21_string.h"

void *trim_chars_list(short *tcSize, const char *trim_chars);
void *trimHelper(const char *src, char *tc);
void *trimedStr(const char *src, short start, short end);
void *emptyStr();

void *s21_trim(const char *src, const char *trim_chars) {
  bool isError = 1;
  short tcSize = 0;
  void *pResult = S21_NULL;

  if (src) {
    if (trim_chars) {
      tcSize = s21_strlen(trim_chars);
    } else if (trim_chars == S21_NULL) {
      tcSize = 0;
    }
    char *tc = trim_chars_list(&tcSize, trim_chars);
    if (tc) {
      pResult = trimHelper(src, tc);
      if (pResult) isError = 0;
    }
  }

  return (isError) ? S21_NULL : pResult;
}

void *trim_chars_list(short *tcSize, const char *trim_chars) {
  char default_chars[7] = " \t\n\v\r\f\0";
  bool isDefault = 0;
  bool isError = 1;
  if (*tcSize == 0) {
    isDefault = 1;
    *tcSize = 6;
  }
  char *tc = calloc(*tcSize + 1, sizeof(char));
  if (tc) {
    if (isDefault) {
      s21_strcpy(tc, default_chars);
      isError = 0;
    } else {
      s21_strcpy(tc, trim_chars);
      isError = 0;
    }
  }
  return (isError) ? S21_NULL : tc;
}

void *trimHelper(const char *src, char *tc) {
  bool isError = 1;
  char *pStart = (char*)src;
  short sLength = s21_strlen(src);
  void *pResult = S21_NULL;

  if (sLength == 0) {
    pResult = emptyStr();
    if (pResult) isError = 0;
  } else {
    short ltrim = 0, rtrim = sLength - 1;

    while (*pStart) {
      for (short i = 0; tc[i] != '\0'; i++) {
        if (ltrim <= sLength && src[ltrim] == tc[i]) {
          ltrim++;
        }
        if (rtrim >= 0 && src[rtrim] == tc[i]) {
          rtrim--;
        }
      }
      pStart++;
    }
    free(tc);
    if (ltrim == sLength) {
      pResult = emptyStr();
      if (pResult) isError = 0;
    } else {
      pResult = trimedStr(src, ltrim, rtrim);
      if (pResult) isError = 0;
    }
  }
  return (isError) ? S21_NULL : pResult;
}

void *trimedStr(const char *src, short start, short end) {
  bool isError = 1;
  void *pResult = S21_NULL;
  int newLength = (end - start) + 1;
  char *pNew = calloc(newLength + 1, sizeof(char));
  if (pNew) {
    pResult = pNew;
    s21_memcpy(pResult, src + start, newLength);
    isError = 0;
  }
  return (isError) ? S21_NULL : pResult;
}

void *emptyStr() {
  bool isError = 1;
  void *pNULL = calloc(1, sizeof(char));
  if (pNULL) {
    s21_memset(pNULL, 0, 1);
    isError = 0;
  }
  return (isError) ? S21_NULL : pNULL;
}
