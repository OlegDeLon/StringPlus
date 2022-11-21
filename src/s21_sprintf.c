#include "s21_sprintf.h"

int s21_sprintf(char* str, const char* format, ...) {
  va_list(input);
  va_start(input, format);
  int i = 0, l = 0, len = 0, init = 0;
  form p;
  while (format[l] != '\0') {
    if (format[l] == '%') {
      forminit(&p);
      l++;
      generalparser(&l, format, &p, input);
      init = i;
      if (s21_strspn(&p.spec, "dioxXcs") >= 1) {
        integer_strings(str, &i, input, &p, &init, &len);
      } else {
        other_flags(str, &i, input, &p, &init, &len);
      }
      i += widthprint(str, &i, p, init, len);
    } else {
      str[i] = format[l];
      l++;
      i++;
    }
  }
  str[i] = '\0';
  va_end(input);
  return i;
}

int widthprint(char* str_out, int* i, form p, int init, int len) {
  int k = 0;
  char insert = ' ', *tmp = S21_NULL;
  if ((s21_strspn(&p.spec, "fgG") >= 1 && p.flag5 == 1 && p.flag1 == 0) ||
      (s21_strspn(&p.spec, "dio") >= 1 && p.flag5 == 1 && p.flag1 == 0 &&
       p.precnull == 1)) {
    insert = '0';
  }
  if (p.precision < 0) {
    if (p.flag5 == 1 && p.flag1 != 1) {
      insert = '0';
    }
    if (abs(p.precision) >= p.width) {
      p.width += abs(p.precision) - p.width;
    } else {
      p.width -= p.width - abs(p.precision);
    }
  }
  if (p.flag1 == 1) {
    init = *i;
  } else {
    if (p.precision < 0) {
      init = *i;
      insert = ' ';
    }
  }
  char* str_width = calloc(p.width + 1, sizeof(char));
  while (k < p.width - len) {
    str_width[k] = insert;
    k++;
  }
  str_width[k] = '\0';
  tmp = (char*)s21_insert(str_out, str_width, init);
  if (tmp) {
    s21_strcpy(str_out, tmp);
    free(tmp);
  }
  free(str_width);
  return k;
}

void checkprec(form* p) {
  if (p->precnull == 1) {
    if (p->spec == 'd' || p->spec == 'i') {
      p->precision = 1;
    }
    if (s21_strspn(&p->spec, "feEgG") >= 1) {
      p->precision = 6;
    }
  } else {
    if (s21_strspn(&p->spec, "feEgG") >= 1 && p->precision < 0) {
      p->precision = 6;
    }
  }
  if ( (p->spec == 'g' || p->spec == 'G') && p->precision == 0 ) {
    p->precision = 1;
  }
}

void other_flags(char* str_out, int* i, va_list input, form* p, int* init,
                 int* len) {
  char* tmp = S21_NULL;
  switch (p->spec) {
    case 'f':
      checkprec(p);
      *len = ftoa(str_out, i, input, *p, init);
      break;
    case 'e':
      checkprec(p);
      *len = expoa(str_out, i, input, *p, init);
      break;
    case 'E':
      checkprec(p);
      *len = expoa(str_out, i, input, *p, init);
      tmp = s21_to_upper(str_out);
      if (tmp) {
        s21_strcpy(str_out, tmp);
        free(tmp);
      }
      break;
    case 'g':
      checkprec(p);
      *len = gotoa(str_out, i, input, *p);
      break;
    case 'G':
      checkprec(p);
      *len = gotoa(str_out, i, input, *p);
      break;
    case 'u':
      checkprec(p);
      *len = itoa(str_out, i, input, *p, init);
      break;
    case 'p':
      *len = itohex(str_out, i, input, *p);
      break;
    case '%':
      str_out[*i] = '%';
      *i += 1;
      *len = 1;
      break;
    case 'n':
      *va_arg(input, int*) = *i;
      break;
  }
}

void integer_strings(char* str_out, int* i, va_list input, form* p, int* init,
                     int* len) {
  char* tmp = S21_NULL;
  switch (p->spec) {
    case 'd':
      checkprec(p);
      *len = itoa(str_out, i, input, *p, init);
      break;
    case 'i':
      checkprec(p);
      *len = itoa(str_out, i, input, *p, init);
      break;
    case 'c':
      checkprec(p);
      str_out[*i] = (char)va_arg(input, int);
      *i += 1;
      *len = 1;
      break;
    case 's':
      checkprec(p);
      tmp = va_arg(input, char*);
      *len = stos(str_out, i, tmp, *p);
      break;
    case 'o':
      checkprec(p);
      *len = itoa(str_out, i, input, *p, init);
      if (p->flag4 == 1) {
        tmp = (char*)s21_insert(str_out, "0", *init);
        if (tmp) {
          s21_strcpy(str_out, tmp);
          free(tmp);
        }
        *i += 1;
        *len += 1;
      }
      break;
    case 'x':
      checkprec(p);
      *len = itohex(str_out, i, input, *p);
      break;
    case 'X':
      checkprec(p);
      *len = itohex(str_out, i, input, *p);
      tmp = s21_to_upper(str_out);
      if (tmp) {
        s21_strcpy(str_out, tmp);
        free(tmp);
      }
      break;
  }
}
