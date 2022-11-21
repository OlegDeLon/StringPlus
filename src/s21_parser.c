#include "s21_parser.h"

void generalparser(int* l, const char* format, form* p, va_list input) {
  parsflag(l, format, p);
  if (format[*l] == '*') {
    if (p->prf == 0) {
      p->width = va_arg(input, int);
    } else {
      p->haram = 1;
    }
    *l += 1;
  } else {
    parswidth(l, format, p);
  }
  if (format[*l] == '.') {
    *l += 1;
    if (format[*l] == '*') {
      p->precision = va_arg(input, int);
      *l += 1;
    } else {
      parsprecision(l, format, p);
    }
  } else {
    p->precnull = 1;
  }
  parslength(l, format, p);
  parsspec(l, format, p);
  if (p->width < 0) {
    p->width *= -1;
    p->flag1 = 1;
  }
}

void parsflag(int* l, const char* format, form* p) {
  s21_size_t len = s21_strspn(format + *l, "+- #0");
  for (s21_size_t i = 0; i < len; i++) {
    switch (*(format + *l)) {
      case '-':
        p->flag1 = 1;
        break;
      case '+':
        p->flag2 = 1;
        break;
      case ' ':
        p->flag3 = 1;
        break;
      case '#':
        p->flag4 = 1;
        break;
      case '0':
        p->flag5 = 1;
        break;
    }
    *l += 1;
  }
}

void parswidth(int* l, const char* format, form* p) {
  s21_size_t len = s21_strspn(format + *l, "1234567890");
  char* buf = calloc(len + 1, sizeof(char));
  s21_size_t i = 0;
  while (i < len) {
    buf[i] = *(format + *l);
    *l += 1;
    i++;
  }
  buf[i + 1] = '\0';
  p->width = atoi(buf);
  free(buf);
}

void parsprecision(int* l, const char* format, form* p) {
  s21_size_t i = 0;
  s21_size_t len = s21_strspn(format + *l, "-1234567890");
  char* buf = calloc(len + 1, sizeof(char));
  while (i < len) {
    buf[i] = *(format + *l);
    *l += 1;
    i++;
  }
  buf[i + 1] = '\0';
  p->precision = atoi(buf);
  free(buf);
}

void parslength(int* l, const char* format, form* p) {
  if (s21_strcspn(format + *l - 1, "lLh") == 1) {
    switch (*(format + *l)) {
      case 'h':
        p->length = 1;
        break;
      case 'l':
        p->length = 2;
        break;
      case 'L':
        p->length = 3;
        break;
    }
    *l += 1;
  }
}

void parsspec(int* l, const char* format, form* p) {
  if (*(format + *l) == '%') {
    p->spec = *(format + *l);
    *l += 1;
  } else {
    if (s21_strcspn(format + *l - 1, "cdeEfgGosuxXpni") == 1) {
      p->spec = *(format + *l);
      *l += 1;
    }
  }
}
