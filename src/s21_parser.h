#ifndef SRC_S21_PARSER_H_
#define SRC_S21_PARSER_H_

#include "s21_string.h"

typedef struct formatt {
    bool flag1;
    bool flag2;
    bool flag3;
    bool flag4;
    bool flag5;
    int width;
    int length;
    int precision;
    char spec;
    bool precnull;
    bool prf;
    bool haram;
} form;

void generalparser(int* l, const char* format, form* p, va_list input);
void parsflag(int* l, const char* format, form* p);
void parswidth(int* l, const char* format, form* p);
void parsprecision(int* l, const char* format, form* p);
void parslength(int* l, const char* format, form* p);
void parsspec(int* l, const char* format, form* p);
#endif  // SRC_S21_PARSER_H_
