#ifndef SRC_S21_GENERAL_H_
#define SRC_S21_GENERAL_H_

#include "s21_string.h"
#include "s21_parser.h"

void forminit(form* p);
int itoa(char* str_out, int* i, va_list input, form p, int* init);
void __int_precision__(char* buf, int* k, int n, int* l, form p);
int stos(char* str_out, int* i, char* input, form p);
int64_t octal(unsigned int input);
int itohex(char* str_out, int* i, va_list input, form p);
void __hex_precision__(char* string, char* buffer, int* num_of_ch, form p);
int ftoa(char* str_out, int* i, va_list arguments, form p, int* init);
int expoa(char* str_out, int* i, va_list arguments, form p, int* init);
void __check_exp__(char* nmbr_e_to_s, int* flag);
void __exp_print_dec__(char* exp, int dec, form p, long double input);
void __e_to_f_correction__(char *buf, form p);
void __fixed_precision__(char *buf, form p);
int gotoa(char* str_out, int* i, va_list arguments, form p);
void sign(char* str_out, int* i, int sig, form p, int* init, int* len);
void imprint(char* str_out, char* buf, int* i, int l, int* len);
void __intlength__(va_list arguments, int64_t* number, form p);
void __floatlength__(va_list arguments, long double* number, form p);
#endif  //  SRC_S21_GENERAL_H_
