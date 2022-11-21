CC = gcc
CFLAGS = -c -Wall -Wextra -Werror -std=c11
LDFLAGS = -O2 -Wall -Wextra -Werror -std=c11
SOURCE = $(wildcard s21_*.c)
HEADER = $(wildcard s21_*.h)
OBJECTS = $(patsubst %.c,%.o, ${SOURCE})
TESTS_SOURCE = $(wildcard test_*.c)
TESTS_OBJECTS = $(patsubst %.c,%.o, ${TESTS_SOURCE})
.LIBPATTERNS:=%.a %.so lib%.so lib%.a
UNAME:=$(shell uname -s)
ifeq ($(UNAME),Linux)
	OPEN_CMD = xdg-open
	ADD_LIB = -lm -lrt -lpthread -D_GNU_SOURCE
	BREW = .
endif
ifeq ($(UNAME),Darwin)
	OPEN_CMD = open
	ADD_LIB = 
	BREW := ${HOME}/homebrew
endif
PATH := $(PATH):$(BREW)/bin
GCOV = --coverage
PROJECTNAME=s21_string

all: clean check $(PROJECTNAME).a test

s21_string.a: ${SOURCE}
	$(CC) $(CFLAGS) $(ADD_LIB) $(SOURCE)
	ar rc $@ ${OBJECTS}
	ranlib $@
	cp $@ lib$@

test: ${TESTS_SOURCE} $(PROJECTNAME).a
	${CC} ${LDFLAGS} -o $@ $^ -lcheck
	./$@

gcov_report: clean lcov ${SOURCE} ${TESTS_SOURCE}
	-mkdir report
	${CC} ${CFLAGS} $(ADD_LIB) ${GCOV} ${SOURCE} ${TESTS_SOURCE}
	${CC} -o $@ ${LFLAGS} ${GCOV} ${OBJECTS} ${TESTS_OBJECTS} -lcheck
	-./$@
	gcov ${TESTS_SOURCE}
	$(shell PATH=$(PATH) which lcov)  -t ${PROJECTNAME} -o ${PROJECTNAME}.info -c -d .
	$(shell PATH=$(PATH) which genhtml) -o report ${PROJECTNAME}.info
	$(OPEN_CMD) ./report/index.html

check: ${SOURCE} ${HEADER}
ifeq ("","$(wildcard ./CPPLINT.cfg)")
	cp -f ../materials/linters/CPPLINT.cfg ./CPPLINT.cfg
endif
	-python ../materials/linters/cpplint.py --extensions=c ${TESTS_SOURCE}
	-python ../materials/linters/cpplint.py --extensions=c ${SOURCE}
	-python ../materials/linters/cpplint.py --extensions=h ${HEADER}

lcov:
ifeq ("", "$(shell PATH=$(PATH) which lcov)")
	$(error Need to install lcov)
endif

clean:
	-rm -rf *.o && rm -rf *.gcno
	-rm -rf *.a && rm -rf *.gcda
	-rm -rf *.info && rm -rf *.gcov
	-rm -rf ./test && rm -rf ./gcov_report
	-rm -rf ./report/

.PHONY: all clean check lcov
