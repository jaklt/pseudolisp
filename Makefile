
VERSION = 0.5.0
NAME = pseudolisp

SRC = ${NAME}.c structs.c error.c execute.c testy.c helpers.c hashovani.c reader.c parser.c funkce.c funkce-sideef.c gc.c
HEADERS = structs.h error.h execute.h testy.h helpers.h hashovani.h parser.h funkce.h gc.h

OBJ = ${SRC:.c=.o}

PARAMS = -std=c99 -Wall -pedantic -D'VERSION="${VERSION}"'
# PARAMS += -g -DDEBUG # je pro gdb/valgrind a DEBUG mod
# PARAMS += -pg # je pro gprof
# PARAMS += -fprofile-arcs -ftest-coverage # je pro gcov

# optimalizace kompilace podle vysledku profielru
# http://www.abclinuxu.cz/blog/kazdy_den_jinak/2007/5/gcc-a-optimalizace-s-profilerem

# strip pseudolisp -> oreze nepotrebne veci


all: ${NAME}

.c.o:
	@echo "  Compiling $<"
	@gcc -c $< ${PARAMS}

${OBJ}: ${SRC}

${NAME}: ${OBJ} ${HEADERS}
	@echo "  Building ${NAME}"
	@gcc -o $@ ${OBJ} ${PARAMS} -lm

clean:
	@echo Cleaning
	@rm -f ${OBJ} *.gcda *.gcno *.gcov gmon.out

dist: ${SRC} ${HEADERS}
	@rm -f ${NAME}-${VERSION}.tar.bz2
	tar cjf ${NAME}-${VERSION}.tar.bz2 * .git .gitignore

znova: clean all

tags:
	ctags -R .
