#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  short isError = 1;
  char *result = S21_NULL;

  if (str && src) {
    s21_size_t length = s21_strlen(src);
    if (start_index <= length) {
      s21_size_t insertLength = s21_strlen(str);
      if ( (length + insertLength) == 0 || (length > 0 && insertLength == 0) ) {
        char *new = calloc(length + 1, sizeof(char));
        if (new) {
          result = new;
          s21_strcpy(new, src);
          isError = 0;
        }
      } else {
        s21_size_t newLength = length + insertLength + 1;
        char *new = calloc(newLength, sizeof(char));
        if (new) {
          result = new;
          if (start_index < length) {
            s21_memcpy(new, (char *)src, start_index);
            s21_memcpy(new + start_index, (char *)str, insertLength);
            s21_memcpy(new + start_index + insertLength, (char *)src + start_index, length - start_index);
            isError = 0;
          } else {
            s21_strcat(new, (char *)src);
            s21_strcat(new, (char *)str);
            isError = 0;
          }
        }
      }
    }
  }

  return (!isError) ? result : S21_NULL;
}
