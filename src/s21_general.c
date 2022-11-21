#include "s21_general.h"

void forminit(form* p) {
  p->flag1 = 0;
  p->flag2 = 0;
  p->flag3 = 0;
  p->flag4 = 0;
  p->flag5 = 0;
  p->length = 0;
  p->precision = 0;
  p->spec = '\0';
  p->width = 0;
  p->precnull = 0;
  p->haram = 0;
  p->prf = 0;
}

int itoa(char* str_out, int* i, va_list arguments, form p, int* init) {
  int64_t number = 0;
  __intlength__(arguments, &number, p);
  int64_t num_tmp = 0;
  int n = 1, len = 0, k = 0, l = 0, sig = 0, j = 0, num = 0;
  unsigned long step = 0, c = 0;
  if (number < 0) {
    number *= -1;
    sig = 1;
  }
  sign(str_out, i, sig, p, init, &len);
  num_tmp = number;
  while ((num_tmp /= 10) > 0) n++;
  char* buf = calloc(n + abs(p.precision) + 2, sizeof(char));
  __int_precision__(buf, &k, n, &l, p);
  while (n > 0 && number != 0) {
    step = 1;
    if (number > 9) {
      for (j = 0; j < n - 1; j++) {
        step = step * 10;
      }
    }
    num = (number - c) / (step);
    buf[k] = num + '0';
    c = c + num * step;
    n--;
    k += 1;
    l++;
  }
  buf[k] = '\0';
  imprint(str_out, buf, i, l, &len);
  free(buf);
  return len;
}

void __int_precision__(char* buf, int* k, int n, int* l, form p) {
  if (p.precision != 0 && p.precision > n) {
    do {
      buf[*k] = '0';
      *k += 1;
      *l += 1;
    } while (*l < p.precision - n);
    buf[*k] = '\0';
  }
}

int stos(char* str_out, int* i, char* input, form p) {
  int len = 0, l_input = s21_strlen(input);
  if (p.precnull == 1 || p.precision < 0) {
    p.precision = l_input;
  }
  while (len < l_input) {
    if (len == p.precision && p.spec == 's' && p.precision >= 0) {
      break;
    }
    str_out[*i] = *input;
    *i += 1;
    input += 1;
    len++;
  }
  return len;
}

int64_t octal(unsigned int input) {
  int64_t octal = 0;
  int64_t i = 1;
  while (input != 0) {
    octal = octal + (input % 8) * i;
    input /= 8;
    i *= 10;
  }
  return octal;
}

int itohex(char* str_out, int* i, va_list arguments, form p) {
  unsigned long input = 0;
  if (p.spec == 'p') {
    p.precision = 10;
    input = va_arg(arguments, unsigned long);
  } else {
    switch (p.length) {
      case 0:
        input = va_arg(arguments, unsigned int);
        break;
      case 1:
        input = (unsigned short int)va_arg(arguments, unsigned);
        break;
      case 2:
        input = va_arg(arguments, unsigned long);
        break;
    }
  }
  int len = 0;
  char* buffer = calloc(2 + (sizeof(unsigned long) * 8), sizeof(char));
  unsigned long ch;
  int num_of_ch = 0;
  do {
    ch = input % 16;
    if (ch > 9) {
      ch = ch + (int)'a' - 10;
    } else {
      ch = ch + (int)'0';
    }
    buffer[num_of_ch] = ch;
    input /= 16;
    num_of_ch++;
  } while (input);
  char* string = calloc(s21_strlen(buffer) + p.precision, sizeof(char));
  __hex_precision__(string, buffer, &num_of_ch, p);
  free(buffer);
  imprint(str_out, string, i, num_of_ch, &len);
  free(string);
  return len;
}

void __hex_precision__(char* string, char* buffer, int* num_of_ch, form p) {
  int prefix = 0, precision = 0;
  if (p.precision > *num_of_ch) {
    precision = p.precision - *num_of_ch;
  }
  if (p.flag4 > 0 || p.spec == 'p') {
    *num_of_ch += 2;
    string[0] = '0';
    string[1] = 'x';
    prefix = 2;
  }
  if (precision > 0) {
    *num_of_ch += precision;
    for (int i = prefix; i < prefix + precision; i++) {
      string[i] = '0';
    }
  }
  int l = 0;
  for (int i = prefix + precision; i < *num_of_ch; i++) {
    string[i] = buffer[*num_of_ch - prefix - precision - l - 1];
    l++;
  }
  string[*num_of_ch] = '\0';
}

int ftoa(char* str_out, int* i, va_list arguments, form p, int* init) {
  long double input;
  int dec = 0, sig = 0, k = 0, len = 0, l = 0, n = 0;
  char *tmp = S21_NULL, *result = S21_NULL;
  __floatlength__(arguments, &input, p);
  int number = abs((int)input);
  if (number != 0) {
    n++;
  }
  result = fcvt(input, p.precision, &dec, &sig);

  tmp = calloc(p.precision + 1, sizeof(char));
  if (tmp) {
    gcvt(input, p.precision, tmp);
    if (tmp[0] == '-' && sig == 0) {
      sig = 1;
    }
    free(tmp);
  }
  sign(str_out, i, sig, p, init, &len);

  char* buf =
      calloc(s21_strlen(result) + abs(dec) + p.precision + 2, sizeof(char));
  int d = dec;
  if ((fabsl(input)) < 1) {
    do {
      buf[k] = '0';
      k += 1;
      d++;
    } while (d < 1);
    buf[k] = '\0';
  }
  s21_strcat(buf, result);
  if (p.precision <= 0) {
    while ((number /= 10) > 0) n++;
    result = ecvt(input, n, &dec, &sig);
    s21_strcat(buf, result);
    if (p.flag4 == 1) {
      if (input < 1 && input > 0) {
        dec++;
      }
      tmp = s21_insert(buf, ".", dec);
      if (tmp) {
        s21_strcpy(buf, tmp);
        free(tmp);
      }
    }
  } else {
    tmp = s21_insert(buf, ".", d);
    if (tmp) {
      s21_strcpy(buf, tmp);
      free(tmp);
    }
  }
  l = s21_strlen(buf);
  imprint(str_out, buf, i, l, &len);
  free(buf);
  return len;
}

int expoa(char* str_out, int* i, va_list arguments, form p, int* init) {
  long double input;
  int dec, sig, flag = 0, len = 0, l = 0;
  __floatlength__(arguments, &input, p);
  char *nmbr_e_to_s = ecvt(input, p.precision + 1, &dec, &sig), *tmp = NULL;
  __check_exp__(nmbr_e_to_s, &flag);
  if (flag == 0) {
    sign(str_out, i, sig, p, init, &len);
    if (p.precision != 0 || p.flag4 == 1) {
      tmp = s21_insert(nmbr_e_to_s, ".", 1);
      s21_strcpy(nmbr_e_to_s, tmp);
      free(tmp);
      dec--;
    }
    char exp[6];
    __exp_print_dec__(exp, dec, p, input);

    s21_strcat(nmbr_e_to_s, exp);
    char* buf = calloc(s21_strlen(nmbr_e_to_s) + 2, sizeof(char));
    s21_strcat(buf, nmbr_e_to_s);
    l = s21_strlen(buf);
    imprint(str_out, buf, i, l, &len);
    free(buf);
  }
  return len;
}

void __check_exp__(char* nmbr_e_to_s, int* flag) {
  if (s21_strcmp(nmbr_e_to_s, "inf") == 0 ||
      s21_strcmp(nmbr_e_to_s, "-inf") == 0 ||
      s21_strcmp(nmbr_e_to_s, "NAN") == 0) {
    *flag += 1;
  }
}

void __exp_print_dec__(char* exp, int dec, form p, long double input) {
  int j = 0;
  if (p.spec == 'e') {
    s21_strcpy(exp, "e");
  } else {
    s21_strcpy(exp, "E");
  }

  if (input < 1 && input > -1 && input != 0) {
    s21_strcat(exp, "-");
    dec *= -1;
  } else {
    s21_strcat(exp, "+");
  }
  if (dec <= 9) {
    exp[2] = '0';
    exp[3] = dec + 48;
    j = 3;
  }
  if (dec > 9) {
    int pow = 1, rank = 1, nmbr = dec;
    while (nmbr / 10 > 0) {
      pow *= 10;
      rank++;
      nmbr /= 10;
    }
    j = 2;
    while (rank > 1) {
      exp[j] = dec / pow + 48;
      dec %= pow;
      pow /= 10;
      j++;
      rank--;
    }
    exp[j] = dec + 48;
  }
  exp[j + 1] = '\0';
}

void __e_to_f_correction__(char* buf, form p) {
  char *pos = s21_strchr(buf, 'e'), *tmp = S21_NULL;
  if (pos) {
    int pow = atoi(pos + 1);

    if (p.precision > pow && pow >= -4) {
      if (pow < 0) {
        pow = pow * (-1);
      }

      char* cor = calloc(s21_strlen(buf) + 1, sizeof(char));
      if (cor) {
        int check_sign = 0;
        if (buf[0] == '-') {
          check_sign = 1;
          s21_memset(cor, '-', 1);
        }
        s21_memset(cor + check_sign, '0', pow);
        char* cor2 = calloc(s21_strlen(buf) + 1, sizeof(char));
        if (cor2) {
          short k = 0;
          for (short i = check_sign; i < pos - buf; i++) {
            if (buf[i] != '.') {
              cor2[k] = buf[i];
              k++;
            }
          }
          s21_strcat(cor, cor2);
          free(cor2);
        }

        tmp = s21_insert(cor, ".", 1 + check_sign);
        if (tmp) {
          s21_strcpy(buf, tmp);
          free(tmp);
        }
        free(cor);
      }
    }
  }
}

void __fixed_precision__(char* buf, form p) {
  char *nullChar = S21_NULL, *newBuf = S21_NULL, *tmp = S21_NULL;
  int shift = 1;
  if (p.flag4) {
    char* pos = s21_strchr(buf, 'e');
    if (pos) {
      nullChar = calloc(p.precision + 1, sizeof(char));
      if (nullChar) {
        if (buf[0] == '-') shift = 2;
        s21_memset(nullChar, '0', p.precision - ((pos - buf) - shift));
        newBuf = s21_insert(buf, nullChar, pos - buf);
        if (newBuf) {
          s21_strcpy(buf, newBuf);
          free(newBuf);
        }
        free(nullChar);
      }
    } else {
      tmp = s21_trim(buf, ".0");
      if (tmp) {
        int length = s21_strlen(tmp);
        nullChar = calloc(p.precision + 1, sizeof(char));
        if (nullChar) {
          s21_memset(nullChar, '0', p.precision - length);
          newBuf = s21_insert(buf, nullChar, s21_strlen(buf));
          if (newBuf) {
            s21_strcpy(buf, newBuf);
            free(newBuf);
          }
          free(nullChar);
        }
        free(tmp);
      }
    }
  }
}

int gotoa(char* str_out, int* i, va_list arguments, form p) {
  long double input;
  __floatlength__(arguments, &input, p);
  int len = 0, l = 0;
  char *buf = calloc(p.precision + 10, sizeof(char)), *tmp = NULL;
  gcvt(input, p.precision, buf);
  __e_to_f_correction__(buf, p);
  __fixed_precision__(buf, p);
  int idxToDel = s21_strcspn(buf, ".");
  if (s21_strspn(buf + idxToDel, ".e") == 2) {
    s21_memmove(&buf[idxToDel], &buf[idxToDel + 1], s21_strlen(buf));
  }
  l = s21_strlen(buf);
  if (p.spec == 'G') {
    tmp = s21_to_upper(buf);
    s21_strcpy(buf, tmp);
    free(tmp);
  }
  imprint(str_out, buf, i, l, &len);
  free(buf);
  return len;
}

void sign(char* str_out, int* i, int sig, form p, int* init, int* len) {
  if (sig == 1) {
    p.flag3 = 0;
    str_out[*init] = '-';
    if (p.flag1 != 1 && p.flag5 == 1) {
      *init += 1;
    }
    *i += 1;
    *len += 1;
  } else {
    if (p.flag2 == 1) {
      p.flag3 = 0;
      str_out[*init] = '+';
      if (p.flag1 != 1 && p.flag5 == 1) {
        *init += 1;
      }
      *i += 1;
      *len += 1;
    }
  }
  if (p.flag3 == 1) {
    str_out[*init] = ' ';
    if (p.flag1 != 1 && p.flag5 == 1) {
      *init += 1;
    }
    *i += 1;
    *len += 1;
  }
  if (s21_strspn(&p.spec, "diu") >= 1) {
    *init -= 1;
  }
}

void imprint(char* str_out, char* buf, int* i, int l, int* len) {
  int k = 0;
  while (k < l) {
    str_out[*i] = buf[k];
    *i += 1;
    k++;
    *len += 1;
  }
}

void __intlength__(va_list arguments, int64_t* number, form p) {
  if (p.spec == 'o') {
    *number = va_arg(arguments, unsigned long);
    *number = octal(*number);
  } else {
    if (p.length == 0) {
      if (p.spec == 'u' || p.spec == 'x') {
        *number = va_arg(arguments, unsigned long);
      } else {
        *number = va_arg(arguments, int);
      }
    }
    if (p.length == 1) {
      if (p.spec == 'u' || p.spec == 'x') {
        *number = (short)va_arg(arguments, unsigned);
      } else {
        *number = (short)va_arg(arguments, int);
      }
    }
    if (p.length == 2) {
      if (p.spec == 'u' || p.spec == 'x') {
        *number = va_arg(arguments, unsigned long);
      } else {
        *number = va_arg(arguments, long int);
      }
    }
  }
}

void __floatlength__(va_list arguments, long double* number, form p) {
  if (p.length == 3) {
    *number = va_arg(arguments, long double);
  } else {
    *number = va_arg(arguments, double);
  }
}
