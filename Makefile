
VERSION = 0.1.0
NAME = pseudolisp

# SRC = funkce.c hashovani.c execute.c reader.c parser.c gc.c funkce-sideef.c
# HEADERS = funkce.h hashovani.h parser.h gc.h

SRC = ${NAME}.c structs.c error.c execute.c testy.c helpers.c
HEADERS = structs.h error.h execute.h testy.h helpers.h

OBJ = ${SRC:.c=.o}

PARAMS = -std=c99 -g -Wall -pedantic -DDEBUG
# -pg je pro gprof
# -g je pro gdb/valgrind
# -D<nazev> odpovida "#define"

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
