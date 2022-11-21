#ifndef SRC_S21_SPRINTF_H_
#define SRC_S21_SPRINTF_H_

#include "s21_general.h"
#include "s21_string.h"

int widthprint(char* str_out, int* i, form p, int init, int len);
void checkprec(form* p);
void other_flags(char* str_out, int* i, va_list input, form* p, int* init,
                      int* len);
void integer_strings(char* str_out, int* i, va_list input, form* p, int* init,
                      int* len);
#endif  // SRC_S21_SPRINTF_H_
