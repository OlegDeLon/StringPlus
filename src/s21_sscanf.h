#ifndef SRC_S21_SSCANF_H_
#define SRC_S21_SSCANF_H_

#include "s21_general.h"
#include "s21_string.h"

int scan(const char* str, va_list arguments, int*i, form p);
void bufprint(const char *str, char *buf, int *i, form p);
void bufclear(char *buf);
void check_conflict(char *buf, int* flag_fail, form p);
void check_prec_and_flags(int *flag_fail, form p);
void int_buf(char *buf, int flag_fail, form p, int *i, va_list arguments);
void char_buf(const char *str, int flag_fail, form p, int *i,
              va_list arguments);
void str_buf(char *buf, int flag_fail, va_list arguments, form p);
void float_buf(char *buf, int flag_fail, form p, va_list arguments);
void hex_buf(char *buf, int flag_fail, form p, va_list arguments);
void octa_buf(char *buf, int flag_fail, form p, va_list arguments);
void semi_buf(char *buf, int flag_fail, form p, int *i, va_list arguments);
#endif  // SRC_S21_SSCANF_H_
