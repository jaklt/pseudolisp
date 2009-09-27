
VERSION = 0.5.1
NAME = pseudolisp
READLINE = 1

SRC = ${NAME}.c structs.c error.c execute.c testy.c helpers.c hashovani.c reader.c parser.c funkce.c funkce-sideef.c gc.c
HEADERS = structs.h error.h execute.h helpers.h hashovani.h parser.h funkce.h gc.h

OBJ = ${SRC:.c=.o}

PARAMS = -std=c99 -Wall -pedantic -D'VERSION="${VERSION}"'
PARAMS+= -D"READLINE=${READLINE}" -lreadline
DEBUG = -g -DDEBUG # je pro gdb/valgrind a DEBUG mod
PROFILER = -pg # je pro gprof
GCOVERAGE = -fprofile-arcs -ftest-coverage # je pro gcov

# optimalizace kompilace podle vysledku profielru
# http://www.abclinuxu.cz/blog/kazdy_den_jinak/2007/5/gcc-a-optimalizace-s-profilerem

# strip pseudolisp -> oreze nepotrebne veci


all: ${NAME}

debug: PARAMS += ${DEBUG}
debug: znova

prof: PARAMS += ${PROFILER}
prof: debug

gcov: PARAMS += ${GCOVERAGE}
gcov: prof

.c.o:
	@echo "  Compiling $<"
	@gcc -c $< ${PARAMS}

${OBJ}: ${SRC}

${NAME}: ${OBJ} ${HEADERS}
	@echo "  Building ${NAME}"
	@gcc -o $@ ${OBJ} ${PARAMS}

clean:
	@echo Cleaning
	@rm -f ${OBJ} *.gcda *.gcno *.gcov gmon.out

dist: ${SRC} ${HEADERS}
	@rm -f ${NAME}-${VERSION}.tar.bz2
	tar cjf ${NAME}-${VERSION}.tar.bz2 * .git .gitignore

znova: clean all

TAGS: ${SRC} ${HEADERS}
	ctags -R .
