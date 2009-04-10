
VERSION = 0.0.5
NAME = pseudolisp

SRC = ${NAME}.c structs.c funkce.c helpers.c testy.c error.c hashovani.c execute.c parser.c gc.c
HEADERS = structs.h funkce.h helpers.h error.h hashovani.h execute.h parser.h gc.h

OBJ = ${SRC:.c=.o}

PARAMS = -std=c99 -g -Wall -pedantic
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
	@rm -f ${OBJ} ${NAME}

dist: ${SRC} ${HEADERS}
	@rm -f ${NAME}-${VERSION}.tar.bz2
	tar cjf ${NAME}-${VERSION}.tar.bz2 * .git .gitignore

znova: clean all

tags: 
	ctags -R .
