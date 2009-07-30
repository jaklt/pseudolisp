
VERSION = 0.1.0
NAME = pseudolisp

# SRC = gc.c
# HEADERS = gc.h

SRC = ${NAME}.c structs.c error.c execute.c testy.c helpers.c hashovani.c reader.c parser.c funkce.c funkce-sideef.c
HEADERS = structs.h error.h execute.h testy.h helpers.h hashovani.h parser.h funkce.h

OBJ = ${SRC:.c=.o}

PARAMS = -std=c99 -Wall -pedantic -D'VERSION="${VERSION}"' -g -DDEBUG
# -pg je pro gprof
# -g je pro gdb/valgrind

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
	@rm -f ${OBJ}

dist: ${SRC} ${HEADERS}
	@rm -f ${NAME}-${VERSION}.tar.bz2
	tar cjf ${NAME}-${VERSION}.tar.bz2 * .git .gitignore

znova: clean all

tags:
	ctags -R .
