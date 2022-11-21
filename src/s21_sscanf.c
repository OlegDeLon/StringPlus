#include "s21_sscanf.h"

int s21_sscanf(const char *str, const char *format, ...) {
  va_list(output);
  va_start(output, format);
  int i = 0, l = 0, k = 0, flag_fail, success = 0;
  form p;
  while (format[l] != '\0') {
    if (format[l] == '%') {
      forminit(&p);
      p.prf = 1;
      l++;
      generalparser(&l, format, &p, output);
      flag_fail = scan(str, output, &i, p);
      if (flag_fail == 1) {
        break;
      }
      while (str[i] == format[l] && str[i] != '%') {
        l++;
        i++;
      }
      k = s21_strspn(&str[i], " ");
      if (k > 0) {
        i += 1;
      }
      k += s21_strcspn(&str[i], " ");
      success += 1;
    } else {
      l++;
    }
  }
  va_end(output);
  return success;
}

int scan(const char *str, va_list arguments, int *i, form p) {
  int flag_fail = 0;
  long long int arg = 0;
  char *buf = malloc(2 * sizeof(char));
  if (s21_strspn(&p.spec, "dixXuosfeEgG") >= 1) {
    bufprint(str, buf, i, p);
    check_prec_and_flags(&flag_fail, p);
    if (p.spec == 's') {
      str_buf(buf, flag_fail, arguments, p);
    } else {
      check_conflict(buf, &flag_fail, p);
      if (s21_strspn(&p.spec, "feEgG")) {
        float_buf(buf, flag_fail, p, arguments);
      }
      if (p.spec == 'd') {
        int_buf(buf, flag_fail, p, i, arguments);
      }
      if (p.spec == 'x' || p.spec == 'X') {
        hex_buf(buf, flag_fail, p, arguments);
      }
      if (p.spec == 'i') {
        semi_buf(buf, flag_fail, p, i, arguments);
      }
      if (p.spec == 'o') {
        octa_buf(buf, flag_fail, p, arguments);
      }
    }
    bufclear(buf);
  } else {
    switch (p.spec) {
      case 'c':
        check_prec_and_flags(&flag_fail, p);
        char_buf(str, flag_fail, p, i, arguments);
        break;
      case 'n':
        check_prec_and_flags(&flag_fail, p);
        if (flag_fail != 1) {
          arg = *i;
          *va_arg(arguments, int *) = arg;
          *i -= 1;
        }
        break;
      case '%':
        *i += 1;
        break;
      case 'p':
        bufprint(str, buf, i, p);
        arg = strtol(buf, NULL, 16);
        *va_arg(arguments, void **) = (void*) arg;
        break;
    }
  }

  free(buf);
  return flag_fail;
}

void bufprint(const char *str, char *buf, int *i, form p) {
  int k = 0, pnull = 0;
  char *tmp = NULL;
  if (p.width == 0) {
    pnull = 1;
    p.width += 1;
  }
  while (str[*i] != ' ' && k != p.width && str[*i] != '\n' && str[*i] != '\t') {
    if (pnull == 1) {
      p.width += 1;
    }
    tmp = realloc(buf, (k + 2) * sizeof(char));
    if (tmp) {
      buf = tmp;
      buf[k] = str[*i];
      k++;
      *i += 1;
    }
  }
  buf[k] = '\0';
}

void bufclear(char *buf) {
  int b = s21_strlen(buf);
  for (int v = 0; v < b; v++) {
    buf[v] = '\0';
  }
}

void check_conflict(char *buf, int* flag_fail, form p) {
  s21_size_t len = s21_strlen(buf);
  if (s21_strspn(buf, "0123456789.-+eE") != len && s21_strspn(buf, "0123456789") == len &&
      (s21_strspn(&p.spec, "deEgGfu")) == 0) {
    *flag_fail = 1;
  }
  if (buf[len - 1] == '.' && s21_strspn(&p.spec, "d")) {
    *flag_fail = 1;
  }
  if (s21_strspn(buf, "0123456789.-+xX") != len &&
      s21_strspn(buf, "0123456789abcdefABCDEF") == len && (s21_strspn(&p.spec, "ixX")) >= 1) {
    *flag_fail = 1;
  }
}

void check_prec_and_flags(int *flag_fail, form p) {
  if (p.precnull != 1) {
    *flag_fail = 1;
  }
  if (p.flag1 != 0 || p.flag2 != 0 || p.flag3 != 0 || p.flag4 != 0 ||
      p.flag5 != 0) {
    *flag_fail = 1;
  }
}

void int_buf(char *buf, int flag_fail, form p, int *i, va_list arguments) {
  int64_t arg = 0;
  if (flag_fail != 1 && p.haram != 1) {
    arg = atoi(buf);
      if (s21_strcspn(buf, ".") != s21_strlen(buf)) {
        *i -= s21_strlen(buf) - s21_strcspn(buf, ".") + 1;
      }
      switch (p.length) {
        case 0:
          *va_arg(arguments, int *) = arg;
          break;
        case 1:
          *va_arg(arguments, short int *) = arg;
          break;
        case 2:
          *va_arg(arguments, long int *) = arg;
          break;
    }
  }
}

void char_buf(const char *str, int flag_fail, form p, int *i,
              va_list arguments) {
  char arg = 0;
  if (flag_fail != 1) {
    arg = str[*i];
    if (p.haram != 1) {
      *va_arg(arguments, char *) = arg;
    }
    *i += 1;
  }
}

void str_buf(char *buf, int flag_fail, va_list arguments, form p) {
  if (flag_fail != 1 && p.haram != 1) {
    s21_strcpy(va_arg(arguments, char *), buf);
  }
}

void float_buf(char *buf, int flag_fail, form p, va_list arguments) {
  long double argf = 0;
if (flag_fail != 1 && p.haram != 1) {
    argf = atof(buf);
    if (p.length == 3) {
      *va_arg(arguments, long double *) = argf;
    }
    if (p.length == 0) {
      *va_arg(arguments, float *) = argf;
    }
    if (p.length == 2) {
      *va_arg(arguments, double *) = argf;
    }
  }
}

void hex_buf(char *buf, int flag_fail, form p, va_list arguments) {
if (flag_fail != 1 && p.haram != 1) {
    switch (p.length) {
      case 0:
        *va_arg(arguments, unsigned int *) = strtol(buf, NULL, 16);
        break;
      case 1:
        *va_arg(arguments, unsigned short int *) = strtol(buf, NULL, 16);
        break;
      case 2:
        *va_arg(arguments, unsigned long int *) = strtol(buf, NULL, 16);
        break;
    }
  }
}

void octa_buf(char *buf, int flag_fail, form p, va_list arguments) {
if (flag_fail != 1 && p.haram != 1) {
    switch (p.length) {
      case 0:
        *va_arg(arguments, unsigned int *) = strtol(buf, NULL, 8);
        break;
      case 1:
        *va_arg(arguments, unsigned short int *) = strtol(buf, NULL, 8);
        break;
      case 2:
        *va_arg(arguments, unsigned long int *) = strtol(buf, NULL, 8);
        break;
    }
  }
}

void semi_buf(char *buf, int flag_fail, form p, int *i, va_list arguments) {
  bool sign = 0;
  if (buf[0] == '-' || buf[0] == '+') {
    sign = 1;
  }
  if (buf[0] == '0' || (buf[1] == '0' && sign == 1)) {
    if ((buf[1] == 'x' || buf[1] == 'X') || ((buf[2] == 'x' || buf[2] == 'X') && sign == 1)) {
      hex_buf(buf, flag_fail, p, arguments);
    } else {
      octa_buf(buf, flag_fail, p, arguments);
    }
  } else {
    int_buf(buf, flag_fail, p, i, arguments);
  }
}
