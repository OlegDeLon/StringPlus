#include <check.h>
#include <stdio.h>
#include <time.h>

#include "s21_string.h"

#define TEST_DATA_SIZE 11

const char testData[TEST_DATA_SIZE][15] = {
    "Hello world\0", "Hello world\n\0", "0123456789\0", "a\n\0", " \n\0",
    "\t\t\n=t\n\0",  "\v\na\v\n",       " \0",          "\n\0",  "\0"};

void *randomStr() {
  int i, size = rand() % 100 + 5;
  char *str = calloc(size, sizeof(char));

  if (str) {
    int spec = size % 25 + 10;
    for (i = 0; i < size; i++) {
      if ((i % spec) == 0) {
        str[i] = rand() % (11 - 9) + 9;  // special symbols
      } else {
        str[i] = rand() % (127 - 32) + 32;
      }
    }
    str[size - 1] = '\0';
  }

  return (void *)str;
}

START_TEST(s21_memchr_test) {
  int length = 0;
  char symbol, search[6] = {'a', 'l', 't', 'e', '4', '\0'};
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    length = strlen(testData[i]);
    for (short k = 0; k < 6; k++) {
      symbol = search[k];
      ck_assert_pstr_eq(s21_memchr(testData[i], symbol, length),
                        memchr(testData[i], symbol, length));
    }
  }

  char *randStr = randomStr();
  if (randStr) {
    symbol = rand() % (127 - 32) + 32;
    length = strlen(randStr);
    ck_assert_pstr_eq(s21_memchr(randStr, symbol, length),
                      memchr(randStr, symbol, length));
    free(randStr);
  }
}
END_TEST

START_TEST(s21_memcmp_test) {
  char str1[20] = {0}, str2[20] = {0};
  int length = 0;
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    length = strlen(testData[i]);
    strcpy(str1, testData[i]), strcpy(str2, testData[i]);
    ck_assert_int_eq(s21_memcmp(str1, str2, length),
                     memcmp(str1, str2, length));
    // clear string
    memset(str1, 0, 20);
    memset(str2, 0, 20);
  }

  char randSymbol = rand() % (127 - 32) + 32;
  memset(str1, randSymbol, 18), memset(str2, randSymbol, 17);
  ck_assert_int_eq(s21_memcmp(str1, str2, 18), memcmp(str1, str2, 18));

  // random
  char *randStr = randomStr();
  if (randStr) {
    length = strlen(randStr);
    char *randStr2 = calloc(length + 1, sizeof(char));
    if (randStr2) {
      strcpy(randStr2, randStr);
      ck_assert_int_eq(s21_memcmp(randStr, randStr2, length),
                       memcmp(randStr, randStr2, length));
      free(randStr2);
    }
    free(randStr);
  }
}
END_TEST

START_TEST(s21_memcpy_test) {
  char s21_dest[25] = {0}, dest[25] = {0};
  int length = 0;
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    length = rand() % 15;
    s21_memcpy(s21_dest, testData[i], length),
        memcpy(dest, testData[i], length);
    ck_assert_pstr_eq(s21_dest, dest);
    // clear string
    memset(s21_dest, 0, 25);
    memset(dest, 0, 25);
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    length = strlen(randStr);
    length = (length > 20) ? 19 : length;
    s21_memcpy(s21_dest, randStr, length), memcpy(dest, randStr, length);
    ck_assert_pstr_eq(s21_dest, dest);
    free(randStr);
  }
}
END_TEST

START_TEST(s21_memmove_test) {
  char str[15] = {0}, s21_dest[20] = {0}, dest[20] = {0};
  int length = 0;
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    strcpy(str, testData[i]);
    length = strlen(str);
    s21_memmove(s21_dest, str, length), memmove(dest, str, length);
    ck_assert_pstr_eq(s21_dest, dest);
    // clear string
    memset(s21_dest, 0, 20);
    memset(dest, 0, 20);
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    length = strlen(randStr) + 1;
    length = (length < 20) ? length : 19;
    s21_memmove(s21_dest, randStr, length), memmove(dest, randStr, length);
    ck_assert_pstr_eq(s21_dest, dest);
    free(randStr);
  }

  // src < desc
  strcpy(str, "tttt");
  strcpy(s21_dest, "0123456789"), strcpy(dest, "0123456789");
  s21_memmove(s21_dest, str, 4), memmove(dest, str, 4);
  ck_assert_pstr_eq(s21_dest, dest);
}
END_TEST

START_TEST(s21_memset_test) {
  char s21_dest[25] = {0}, dest[25] = {0};
  char symbol[6] = {'c', '5', '\0', '\n', '\t', '\v'};
  int length = 0;
  for (short i = 0; i < 6; i++) {
    for (short step = 0; step <= 5; step++) {
      length = rand() % 25;
      s21_memset(s21_dest, symbol[i], length), memset(dest, symbol[i], length);
      ck_assert_pstr_eq(s21_dest, dest);
      // clear string
      memset(s21_dest, 0, 25);
      memset(dest, 0, 25);
    }
  }

  // random
  char randSymbol = rand() % (127 - 32) + 32;
  s21_memset(s21_dest, randSymbol, 20), memset(dest, randSymbol, 20);
  ck_assert_pstr_eq(s21_dest, dest);
}
END_TEST

START_TEST(s21_strcat_test) {
  char str[15] = {0}, s21_dest[50] = {0}, dest[50] = {0};
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    strcpy(str, testData[i]);
    strcpy(dest, testData[i]);
    strcpy(s21_dest, testData[i]);
    ck_assert_pstr_eq(s21_strcat(s21_dest, str), strcat(dest, str));
    // clear string
    memset(s21_dest, 0, 50);
    memset(dest, 0, 50);
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    char randSymbol = rand() % (127 - 32) + 32;
    char *randStr2 = calloc(strlen(randStr) + 10, sizeof(char));
    char *randStr3 = calloc(strlen(randStr) + 10, sizeof(char));
    if (randStr2 && randStr3) {
      memset(randStr2, randSymbol, 8);
      memset(randStr3, randSymbol, 8);
      ck_assert_pstr_eq(s21_strcat(randStr2, randStr),
                        strcat(randStr3, randStr));
    }
    if (randStr2) free(randStr2);
    if (randStr3) free(randStr3);

    free(randStr);
  }
}
END_TEST

START_TEST(s21_strncat_test) {
  char str[15] = {0}, s21_dest[50] = {0}, dest[50] = {0};
  int length = 0;
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    strcpy(str, testData[i]);
    strcpy(dest, testData[i]);
    strcpy(s21_dest, testData[i]);
    length = rand() % 20;
    ck_assert_pstr_eq(s21_strncat(s21_dest, str, length),
                      strncat(dest, str, length));
    // clear string
    memset(s21_dest, 0, 50);
    memset(dest, 0, 50);
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    char randSymbol = rand() % (127 - 32) + 32;
    memset(s21_dest, randSymbol, 10), memset(dest, randSymbol, 10);

    length = s21_strlen(randStr);
    length = (length < 31) ? length : 30;
    ck_assert_pstr_eq(s21_strncat(s21_dest, randStr, length),
                      strncat(dest, randStr, length));
    free(randStr);
  }
}
END_TEST

START_TEST(s21_strchr_test) {
  char symbol, str[15] = {0}, search[6] = {'a', 'l', 't', 'e', '6', '\0'};
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    strcpy(str, testData[i]);
    for (short k = 0; k < 6; k++) {
      symbol = search[k];
      ck_assert_pstr_eq(s21_strchr(str, symbol), strchr(str, symbol));
    }
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    symbol = rand() % (127 - 32) + 32;
    ck_assert_pstr_eq(s21_strchr(randStr, symbol), strchr(randStr, symbol));
    free(randStr);
  }
}
END_TEST

START_TEST(s21_strcmp_test) {
  char str1[20] = {0}, str2[20] = {0};
  int length = 0, rescmp1, rescmp2;
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    strcpy(str1, testData[i]), strcpy(str2, testData[i]);
    ck_assert_int_eq(s21_strcmp(str1, str2), strcmp(str1, str2));
    // clear string
    memset(str1, 0, 20);
    memset(str2, 0, 20);
  }

  char randSymbol = rand() % (127 - 32) + 32;
  memset(str1, randSymbol, 18), memset(str2, randSymbol, 17);
  rescmp1 = s21_strcmp(str1, str2);
  rescmp2 = strcmp(str1, str2);
  if (rescmp1 < 0) {
    ck_assert_int_le(s21_strcmp(str1, str2), strcmp(str1, str2));
  }
  if (rescmp1 > 0) {
    ck_assert_int_ge(s21_strcmp(str1, str2), strcmp(str1, str2));
  }
  if (rescmp1 == 0) {
    ck_assert_int_eq(s21_strcmp(str1, str2), strcmp(str1, str2));
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    length = strlen(randStr);
    char *randStr2 = calloc(length + 1, sizeof(char));
    if (randStr2) {
      strcpy(randStr2, randStr);
      rescmp1 = s21_strcmp(randStr, randStr2);
      rescmp2 = strcmp(randStr, randStr2);
      if (rescmp1 < 0) {
        ck_assert_int_le(s21_strcmp(randStr, randStr2),
                         strcmp(randStr, randStr2));
      }
      if (rescmp1 > 0) {
        ck_assert_int_ge(s21_strcmp(randStr, randStr2),
                         strcmp(randStr, randStr2));
      }
      if (rescmp1 == 0) {
        ck_assert_int_eq(s21_strcmp(randStr, randStr2),
                         strcmp(randStr, randStr2));
      }
      free(randStr2);
    }
    free(randStr);
  }
}
END_TEST

START_TEST(s21_strncmp_test) {
  char str1[20] = {0}, str2[20] = {0};
  int length = 0, rescmp1 = 0, rescmp2 = 0;
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    length = rand() % 20;
    strcpy(str1, testData[i]), strcpy(str2, testData[i]);
    rescmp1 = s21_strcmp(str1, str2);
    rescmp2 = strcmp(str1, str2);
    if (rescmp1 < 0) {
      ck_assert_int_le(s21_strcmp(str1, str2), strcmp(str1, str2));
    }
    if (rescmp1 > 0) {
      ck_assert_int_ge(s21_strcmp(str1, str2), strcmp(str1, str2));
    }
    if (rescmp1 == 0) {
      ck_assert_int_eq(s21_strcmp(str1, str2), strcmp(str1, str2));
    }
    // clear string
    memset(str1, 0, 20);
    memset(str2, 0, 20);
  }

  char randSymbol = rand() % (127 - 32) + 32;
  memset(str1, randSymbol, 18), memset(str2, randSymbol, 17);
  rescmp1 = s21_strncmp(str1, str2, 18);
  rescmp2 = strncmp(str1, str2, 18);
  if (rescmp1 < 0) {
    ck_assert_int_le(s21_strncmp(str1, str2, 18), strncmp(str1, str2, 18));
  }
  if (rescmp1 > 0) {
    ck_assert_int_ge(s21_strncmp(str1, str2, 18), strncmp(str1, str2, 18));
  }
  if (rescmp1 == 0) {
    ck_assert_int_eq(s21_strncmp(str1, str2, 18), strncmp(str1, str2, 18));
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    length = strlen(randStr);
    char *randStr2 = calloc(length + 1, sizeof(char));
    if (randStr2) {
      strcpy(randStr2, randStr);
      rescmp1 = s21_strncmp(str1, str2, 18);
      rescmp2 = strncmp(str1, str2, 18);
      if (rescmp1 < 0) {
        ck_assert_int_le(s21_strncmp(str1, str2, 18), strncmp(str1, str2, 18));
      }
      if (rescmp1 > 0) {
        ck_assert_int_ge(s21_strncmp(str1, str2, 18), strncmp(str1, str2, 18));
      }
      if (rescmp1 == 0) {
        ck_assert_int_eq(s21_strncmp(str1, str2, 18), strncmp(str1, str2, 18));
      }
      free(randStr2);
    }
    free(randStr);
  }
}
END_TEST

START_TEST(s21_strcpy_test) {
  char s21_dest[20] = {0}, dest[20] = {0};
  int length = 0;
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    ck_assert_pstr_eq(s21_strcpy(s21_dest, testData[i]),
                      strcpy(dest, testData[i]));
    // clear string
    memset(s21_dest, 0, 20);
    memset(dest, 0, 20);
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    length = strlen(randStr);
    char *randStr2 = calloc(length + 1, sizeof(char));
    char *randStr3 = calloc(length + 1, sizeof(char));
    if (randStr2 && randStr3) {
      memset(randStr2, 0, length);
      memset(randStr3, 0, length);
      ck_assert_pstr_eq(s21_strcpy(randStr2, randStr),
                        strcpy(randStr3, randStr));
    }
    if (randStr2) free(randStr2);
    if (randStr3) free(randStr3);

    free(randStr);
  }
}
END_TEST

START_TEST(s21_strncpy_test) {
  char s21_dest[20] = {0}, dest[20] = {0};
  int length = 0;
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    length = rand() % 20;
    s21_strncpy(s21_dest, testData[i], length),
        strncpy(dest, testData[i], length);
    ck_assert_pstr_eq(s21_dest, dest);
    // clear string
    memset(s21_dest, 0, 20);
    memset(dest, 0, 20);
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    length = strlen(randStr);
    length = (length < 20) ? length : 19;
    s21_strncpy(s21_dest, randStr, length), strncpy(dest, randStr, length);
    ck_assert_pstr_eq(s21_dest, dest);
    free(randStr);
  }
}
END_TEST

START_TEST(s21_strcspn_test) {
  char search[TEST_DATA_SIZE][6] = {"ldw\0", "ldt\0", "9876\0", "a",  "\n",
                                    "=",     "\v",    " ",      "\n", ""};
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    ck_assert_int_eq(s21_strcspn(testData[i], search[i]),
                     strcspn(testData[i], search[i]));
  }

  ck_assert_int_eq(s21_strcspn("test data string", "lo"),
                   strcspn("test data string", "lo"));

  // random
  char *randStr = randomStr();
  if (randStr) {
    short i = rand() % 11;
    ck_assert_int_eq(s21_strcspn(randStr, search[i]),
                     strcspn(randStr, search[i]));
    ck_assert_int_eq(s21_strcspn(search[i], randStr),
                     strcspn(search[i], randStr));
    free(randStr);
  }
}
END_TEST

START_TEST(s21_strerror_test) {
  int errnum = -1;
  ck_assert_pstr_eq(s21_strerror(errnum), strerror(errnum));

  // random
  for (int step = 0; step < 10; step++) {
    errnum = rand() % 1000 + 100;
    ck_assert_pstr_eq(s21_strerror(errnum), strerror(errnum));

    errnum = rand() % 100;
    ck_assert_pstr_eq(s21_strerror(errnum), strerror(errnum));
  }
}
END_TEST

START_TEST(s21_strlen_test) {
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    ck_assert_int_eq(s21_strlen(testData[i]), strlen(testData[i]));
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    ck_assert_int_eq(s21_strlen(randStr), strlen(randStr));
    free(randStr);
  }
}
END_TEST

START_TEST(s21_strpbrk_test) {
  short i = 0;
  char search[TEST_DATA_SIZE][6] = {"ldw\0", "ldt\0", "469\0", "a",  "\n",
                                    "=",     "\v",    " ",     "\n", ""};
  for (i = 0; i < TEST_DATA_SIZE; i++) {
    ck_assert_pstr_eq(s21_strpbrk(testData[i], search[i]),
                      strpbrk(testData[i], search[i]));
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    int length = strlen(randStr);
    i = rand() % length;
    if (i + 10 > length) {
      i -= (i + 10) - length;
    }
    char sym[10] = {0};
    s21_memcpy(sym, randStr + i, 9);
    int test_key = rand() % TEST_DATA_SIZE,
        search_key = rand() % TEST_DATA_SIZE;
    ck_assert_pstr_eq(s21_strpbrk(randStr, search[search_key]),
                      strpbrk(randStr, search[search_key]));
    ck_assert_pstr_eq(s21_strpbrk(testData[test_key], randStr),
                      strpbrk(testData[test_key], randStr));
    ck_assert_pstr_eq(s21_strpbrk(randStr, sym), strpbrk(randStr, sym));
    free(randStr);
  }
}
END_TEST

START_TEST(s21_strrchr_test) {
  char symbol, search[6] = {'a', 'l', 't', 'e', '4', '\0'};
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    for (short k = 0; k < 6; k++) {
      symbol = search[k];
      ck_assert_pstr_eq(s21_strrchr(testData[i], symbol),
                        strrchr(testData[i], symbol));
    }
  }

  char *randStr = randomStr();
  if (randStr) {
    symbol = rand() % (127 - 32) + 32;
    ck_assert_pstr_eq(s21_strrchr(randStr, symbol), strrchr(randStr, symbol));
    free(randStr);
  }
}
END_TEST

START_TEST(s21_strspn_test) {
  short i = 0;
  char search[TEST_DATA_SIZE][6] = {"lo\0", "ld\0", "67\0", "\v", "\n",
                                    "t",    "a",    " ",    "\n", ""};
  for (i = 0; i < TEST_DATA_SIZE; i++) {
    ck_assert_int_eq(s21_strspn(testData[i], search[i]),
                     strspn(testData[i], search[i]));
  }

  // random
  char *randStr = randomStr();
  if (randStr) {
    int length = strlen(randStr);
    i = rand() % length;
    if (i + 4 > length) {
      i -= (i + 4) - length;
    }
    char sym[4] = {0};
    s21_memcpy(sym, randStr + i, 3);
    int test_key = rand() % TEST_DATA_SIZE,
        search_key = rand() % TEST_DATA_SIZE;
    ck_assert_int_eq(s21_strspn(randStr, search[search_key]),
                     strspn(randStr, search[search_key]));
    ck_assert_int_eq(s21_strspn(testData[test_key], randStr),
                     strspn(testData[test_key], randStr));
    ck_assert_int_eq(s21_strspn(randStr, sym), strspn(randStr, sym));
    free(randStr);
  }
}
END_TEST

START_TEST(s21_strstr_test) {
  short i = 0;
  char search[TEST_DATA_SIZE][6] = {"llo\0", "rld\0", "789\0", "\v", "\n",
                                    "=t",    "a",     " ",     "\n", ""};
  for (i = 0; i < TEST_DATA_SIZE; i++) {
    ck_assert_pstr_eq(s21_strstr(testData[i], search[i]),
                      strstr(testData[i], search[i]));
  }

  ck_assert_pstr_eq(s21_strstr("test data", "to"), strstr("test data", "to"));

  // random
  char *randStr = randomStr();
  if (randStr) {
    int length = strlen(randStr);
    i = rand() % length;
    if (i + 4 > length) {
      i -= (i + 4) - length;
    }
    char sym[4] = {0};
    s21_memcpy(sym, randStr + i, 3);
    int test_key = rand() % TEST_DATA_SIZE,
        search_key = rand() % TEST_DATA_SIZE;
    ck_assert_pstr_eq(s21_strstr(randStr, search[search_key]),
                      strstr(randStr, search[search_key]));
    ck_assert_pstr_eq(s21_strstr(testData[test_key], randStr),
                      strstr(testData[test_key], randStr));
    ck_assert_pstr_eq(s21_strstr(randStr, sym), strstr(randStr, sym));
    free(randStr);
  }
}
END_TEST

START_TEST(s21_strtok_test) {
  char str1[50] = "/test1/test2/test3/\ntest4\n/test5\n/\ttest6\0", str2[50];
  char *tok1, *tok2;
  strcpy(str2, str1);

  tok1 = s21_strtok(str1, "/");
  tok2 = strtok(str2, "/");
  while (tok1 && tok2) {
    ck_assert_pstr_eq(tok1, tok2);
    tok1 = s21_strtok(S21_NULL, "/");
    tok2 = strtok(S21_NULL, "/");
  }

  strcpy(str1, "st.\0ri,ng");
  strcpy(str2, str1);
  tok1 = s21_strtok(str1, ".,");
  tok2 = strtok(str2, ".,");
  while (tok1 && tok2) {
    ck_assert_pstr_eq(tok1, tok2);
    tok1 = s21_strtok(S21_NULL, ".,");
    tok2 = strtok(S21_NULL, ".,");
  }

  char *randStr = randomStr();
  if (randStr) {
    srand(time(S21_NULL));
    char sym[2] = {0};
    sym[1] = rand() % (127 - 32) + 32;
    tok1 = s21_strtok(randStr, sym);
    tok2 = strtok(randStr, sym);
    while (tok1 && tok2) {
      ck_assert_pstr_eq(tok1, tok2);
      tok1 = s21_strtok(S21_NULL, sym);
      tok2 = strtok(S21_NULL, sym);
    }
  }
}
END_TEST

START_TEST(s21_sprintf_test) {
  short length = 100;
  char *str1 = calloc(length, sizeof(char));
  char *str2 = calloc(length, sizeof(char));
  int res1 = 0, res2 = 0;

  if (str1 && str2) {
    // TEST specifier "с"
    res1 = s21_sprintf(str1, "%c %c %c", ' ', '#', '@');
    res2 = sprintf(str2, "%c %c %c", ' ', '#', '@');
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "d"
    long int ex9 = 11;
    res1 = s21_sprintf(str1, "%ld%*.5d  % d   %+d %*d%-2.d", ex9, -2, 3, 2, 46,
                       4, -57, 9);
    res2 = sprintf(str2, "%ld%*.5d  % d   %+d %*d%-2.d", ex9, -2, 3, 2, 46, 4,
                   -57, 9);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    res1 = 0, res2 = 0;
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "i"
    short int ex1 = 11;
    res1 = s21_sprintf(str1, "%hi %.5i %0i", ex1, -46, 57);
    res2 = sprintf(str2, "%hi %.5i %0i", ex1, -46, 57);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "e"
    res1 = s21_sprintf(str1, "%.*e %e %0+e %e", -12, 9.34656, 0.00000000004,
                       1.5732689, 3.9265e+2);
    res2 = sprintf(str2, "%.*e %e %0+e %e", -12, 9.34656, 0.00000000004,
                   1.5732689, 3.9265e+2);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "E"
    res1 = s21_sprintf(str1, "%E %E %E %E", 9.34656, 0.6740046, -1.5732689,
                       3.9265E+2);
    res2 =
        sprintf(str2, "%E %E %E %E", 9.34656, 0.6740046, -1.5732689, 3.9265E+2);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "f"
    long double ex2 = -7.0004522;
    res1 = s21_sprintf(str1, "%Lf %#f %.2f %*f %.*f %#.f %f", ex2, 0.245236,
                       98754.3456, 12, 2.32, -3, 11.1111, 0.34, -0.00);
    res2 = sprintf(str2, "%Lf %#f %.2f %*f %.*f %#.f %f", ex2, 0.245236,
                   98754.3456, 12, 2.32, -3, 11.1111, 0.34, -0.00);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "G"
    res1 = s21_sprintf(str1, "%G %G %.2G %*G", -7.0004522, 0.245236, 98754.3456,
                       12, 2.32);
    res2 = sprintf(str2, "%G %G %.2G %*G", -7.0004522, 0.245236, 98754.3456, 12,
                   2.32);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "g"
    long double ex6 = -7.0004522;
    res1 = s21_sprintf(str1, "%Lg %g %.2g %*g %#g", ex6, -0.04, 98754.3456, 12,
                       2.32, 0.00000563);
    res2 = sprintf(str2, "%Lg %g %.2g %*g %#g", ex6, -0.04, 98754.3456, 12,
                   2.32, 0.00000563);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "o"
    long int ex3 = 012;
    long res1 = s21_sprintf(str1, "%lo %o %#o", ex3, 067, 045);
    res2 = sprintf(str2, "%lo %o %#o", ex3, 067, 045);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "s"
    res1 = s21_sprintf(str1, "%.*s %.3s %s", -2, "Hello", " world", "!");
    res2 = sprintf(str2, "%.*s %.3s %s", -2, "Hello", " world", "!");
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "u"
    unsigned short ex7 = 85;
    unsigned long ex8 = 453;
    res1 = s21_sprintf(str1, "%hu %lu %u", ex7, ex8, (unsigned)56432322);
    res2 = sprintf(str2, "%hu %lu %u", ex7, ex8, (unsigned)56432322);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "x"
    unsigned short ex4 = 0x344;
    unsigned long ex5 = 0x312;
    res1 = s21_sprintf(str1, "%hx %lx %.20x", ex4, ex5, 0xf42ce);
    res2 = sprintf(str2, "%hx %lx %.20x", ex4, ex5, 0xf42ce);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "X"
    res1 = s21_sprintf(str1, "%X %X %X", 0xFE3324, 0x31DF5, 0xF4C5E7A5);
    res2 = sprintf(str2, "%X %X %X", 0xFE3324, 0x31DF5, 0xF4C5E7A5);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    memset(str1, 0, length), memset(str2, 0, length);

    // TEST specifier "p" and "n" and "%"
    int n1 = 0, n2 = 0;
    res1 = s21_sprintf(str1, "%p %p %% %n", &res1, &res2, &n1);
    res2 = sprintf(str2, "%p %p %% %n", &res1, &res2, &n2);
    ck_assert_str_eq(str1, str2);
    ck_assert_int_eq(res1, res2);
    ck_assert_int_eq(n1, n2);
    memset(str1, 0, length), memset(str2, 0, length);
  }

  if (str1) free(str1);
  if (str2) free(str2);
}
END_TEST

START_TEST(s21_sscanf_test) {
  // TEST %d
  short int a = 0, a1 = 0;
  int a4 = 0, a5 = 0, res1 = 0, res2 = 0;
  long int a2 = 0, a3 = 0;
  res1 = sscanf("123456789, -12345, 1234", "%hd, %ld, %2d", &a, &a2, &a4);
  res2 = s21_sscanf("123456789, -12345, 1234", "%hd, %ld, %2d", &a1, &a3, &a5);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a, a1);
  ck_assert_int_eq(a2, a3);
  ck_assert_int_eq(a4, a5);
  res1 = 0, res2 = 0;
  // TEST %x
  unsigned int b, b1, b6, b7;
  unsigned short int b2, b3;
  unsigned long int b4, b5;
  res1 = sscanf("0x10c, 10c, -0x10c, 0x10c10c10c", "%x, %hx, %lx, %x", &b, &b2,
                &b4, &b6);
  res2 = s21_sscanf("0x10c, 10c, -0x10c, 0x10c10c10c", "%x, %hx, %lx, %x", &b1,
                    &b3, &b5, &b7);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(b, b1);
  ck_assert_int_eq(b2, b3);
  ck_assert_int_eq(b4, b5);
  ck_assert_int_eq(b6, b7);
  res1 = 0, res2 = 0;
  // TEST %i
  int c, c1, c2, c3, c4, c5, c6, c7;
  res1 =
      sscanf("123, 0x10c, 0414, -0x10c", "%i, %i, %i, %i", &c, &c2, &c4, &c6);
  res2 = s21_sscanf("123, 0x10c, 0414, -0x10c", "%i, %i, %i, %i", &c1, &c3, &c5,
                    &c7);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(c, c1);
  ck_assert_int_eq(c2, c3);
  ck_assert_int_eq(c4, c5);
  ck_assert_int_eq(c6, c7);
  res1 = 0, res2 = 0;
  // TEST %o
  unsigned int d, d1;
  unsigned short int d2, d3;
  unsigned long int d4, d5;
  res1 = sscanf("0414, 414, -414", "%o, %ho, %lo", &d, &d2, &d4);
  res2 = s21_sscanf("0414, 414, -414", "%o, %ho, %lo", &d1, &d3, &d5);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(d, d1);
  ck_assert_int_eq(d2, d3);
  ck_assert_int_eq(d4, d5);
  res1 = 0, res2 = 0;
  // TEST %c
  char e, e1, e2, e3;
  res1 = sscanf("a, g", "%c, %c", &e, &e2);
  res2 = s21_sscanf("a, g", "%c, %c", &e1, &e3);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(e, e1);
  ck_assert_int_eq(e2, e3);
  res1 = 0, res2 = 0;
  // TEST %s
  char s[50] = {0}, s1[30] = {0};
  char s2[30] = {0}, s3[30] = {0};
  res1 = sscanf("atata atata", "%s %3s", s, s2);
  res2 = s21_sscanf("atata atata", "%s %3s", s1, s3);
  ck_assert_int_eq(res1, res2);
  ck_assert_pstr_eq(s, s1);
  ck_assert_pstr_eq(s2, s3);
  res1 = 0, res2 = 0;
  // TEST %f
  float f, f1;
  long double f2, f3;
  double f4, f5;
  res1 = sscanf("2.23, 4.124, 3.3333", "%f, %Lf, %lf", &f, &f2, &f4);
  res2 = s21_sscanf("2.23, 4.124, 3.3333", "%f, %Lf, %lf", &f1, &f3, &f5);
  ck_assert_int_eq(res1, res2);
  ck_assert_float_eq(f, f1);
  ck_assert_double_eq(f2, f3);
  ck_assert_double_eq(f4, f5);
  res1 = 0, res2 = 0;
  // TEST %n
  int n, n1;
  char m[10], m1[10];
  res1 = sscanf("atata ala", "%s %n %s", m, &n, s1);
  res2 = s21_sscanf("atata ala", "%s %n %s", m1, &n1, s1);
  ck_assert_int_eq(n, n1);
  res1 = 0, res2 = 0;
  // TEST %p
  void *p2 = NULL, *p3 = NULL;
  res1 = sscanf("og % 0xdf dok", "%s %% %p %s", s1, &p2, s1);
  res2 = s21_sscanf("og % 0xdf dok", "%s %% %p %s", s1, &p3, s1);
  ck_assert_ptr_eq(p2, p3);
  res1 = 0, res2 = 0;

  END_TEST
}
START_TEST(s21_to_upper_test) {
  char *str;
  char result[TEST_DATA_SIZE][15] = {
      "HELLO WORLD\0", "HELLO WORLD\n\0", "0123456789\0", "A\n\0", " \n\0",
      "\t\t\n=T\n\0",  "\v\nA\v\n",       " \0",          "\n\0",  "\0"};
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    str = s21_to_upper(testData[i]);
    if (str) {
      ck_assert_pstr_eq(str, result[i]);
      free(str);
    }
  }
}
END_TEST

START_TEST(s21_to_lower_test) {
  char *str;
  char result[TEST_DATA_SIZE][15] = {
      "hello world\0", "hello world\n\0", "0123456789\0", "a\n\0", " \n\0",
      "\t\t\n=t\n\0",  "\v\na\v\n",       " \0",          "\n\0",  "\0"};
  for (short i = 0; i < TEST_DATA_SIZE; i++) {
    str = s21_to_lower(testData[i]);
    if (str) {
      ck_assert_pstr_eq(str, result[i]);
      free(str);
    }
  }

  str = s21_to_lower("HELLO WORLD");
  if (str) {
    ck_assert_pstr_eq(str, "hello world");
    free(str);
  }

  str = s21_to_lower("TeSt\nTeSt\ntEsT\v");
  if (str) {
    ck_assert_pstr_eq(str, "test\ntest\ntest\v");
    free(str);
  }
}
END_TEST

START_TEST(s21_insert_test) {
  char *str;

  str = s21_insert("Heworld", "llo ", 2);
  if (str) {
    ck_assert_pstr_eq(str, "Hello world");
    free(str);
  }

  str = s21_insert("", "", 0);
  if (str) {
    ck_assert_pstr_eq(str, "");
    free(str);
  }

  str = s21_insert("Hello world", ", AI!", 11);
  if (str) {
    ck_assert_pstr_eq(str, "Hello world, AI!");
    free(str);
  }

  str = s21_insert("Hello world", "", 15);
  ck_assert_pstr_eq(str, S21_NULL);
}
END_TEST

START_TEST(s21_trim_test) {
  char *str;

  str = s21_trim("  \t\tTest string\t\t  \n", S21_NULL);
  if (str) {
    ck_assert_pstr_eq(str, "Test string");
    free(str);
  }

  str = s21_trim(" \t\t\n \n\t\n", S21_NULL);
  if (str) {
    ck_assert_pstr_eq(str, "");
    free(str);
  }

  str = s21_trim("a\n", S21_NULL);
  if (str) {
    ck_assert_pstr_eq(str, "a");
    free(str);
  }

  str = s21_trim("", "a");
  if (str) {
    ck_assert_pstr_eq(str, "");
    free(str);
  }

  str = s21_trim(S21_NULL, "a");
  ck_assert_pstr_eq(str, S21_NULL);

  str = s21_trim("*-***  --Hello world--  *-***", " *-");
  if (str) {
    ck_assert_pstr_eq(str, "Hello world");
    free(str);
  }

  str = s21_trim("*_* trim test Hello world test trim *_*", "t r*s_eim");
  if (str) {
    ck_assert_pstr_eq(str, "Hello world");
    free(str);
  }
}
END_TEST

int main(void) {
  long ltime = time(S21_NULL);
  int stime = (unsigned int)ltime / 2;
  srand(stime);

  Suite *s1 = suite_create("s21_string");
  TCase *s21_stringCase = tcase_create("s21_string_comparativeTest");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, s21_stringCase);
  tcase_add_test(s21_stringCase, s21_memchr_test);
  tcase_add_test(s21_stringCase, s21_memcmp_test);
  tcase_add_test(s21_stringCase, s21_memcpy_test);
  tcase_add_test(s21_stringCase, s21_memmove_test);
  tcase_add_test(s21_stringCase, s21_memset_test);
  tcase_add_test(s21_stringCase, s21_strcat_test);
  tcase_add_test(s21_stringCase, s21_strncat_test);
  tcase_add_test(s21_stringCase, s21_strchr_test);
  tcase_add_test(s21_stringCase, s21_strcmp_test);
  tcase_add_test(s21_stringCase, s21_strncmp_test);
  tcase_add_test(s21_stringCase, s21_strcpy_test);
  tcase_add_test(s21_stringCase, s21_strncpy_test);
  tcase_add_test(s21_stringCase, s21_strcspn_test);
  tcase_add_test(s21_stringCase, s21_strerror_test);
  tcase_add_test(s21_stringCase, s21_strlen_test);
  tcase_add_test(s21_stringCase, s21_strpbrk_test);
  tcase_add_test(s21_stringCase, s21_strrchr_test);
  tcase_add_test(s21_stringCase, s21_strspn_test);
  tcase_add_test(s21_stringCase, s21_strstr_test);
  tcase_add_test(s21_stringCase, s21_strtok_test);
  tcase_add_test(s21_stringCase, s21_sprintf_test);
  tcase_add_test(s21_stringCase, s21_sscanf_test);
  tcase_add_test(s21_stringCase, s21_to_upper_test);
  tcase_add_test(s21_stringCase, s21_to_lower_test);
  tcase_add_test(s21_stringCase, s21_insert_test);
  tcase_add_test(s21_stringCase, s21_trim_test);

  srunner_run_all(sr, CK_VERBOSE);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
