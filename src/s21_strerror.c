#include "s21_strerror.h"

#include "s21_string.h"

char *s21_strerror(int errnum) {
  const char *pMessage = S21_NULL;
  static char buf[20] = {0};
  if ((errnum >= 0) && (errnum <= MAX_ERROR_NUM)) {
    char *error[MAX_ERROR_NUM + 1];
#if defined(__APPLE__)
    SYS_ERROR_LIST_MAC
#elif defined(__linux__)
    SYS_ERROR_LIST_LINUX
#else
    SYS_ERROR_LIST_WINDOWS
#endif
    pMessage = error[errnum];
  } else {
#if defined(__APPLE__)
    s21_sprintf(buf, "Unknown error: %d", errnum);
#else
    s21_sprintf(buf, "Unknown error %d", errnum);
#endif
    pMessage = buf;
  }
  return (char *)pMessage;
}
