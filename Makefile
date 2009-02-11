
VERSION = 0.0.3
NAME = pseudolisp

SRC = ${NAME}.c structs.c funkce.c helpers.c testy.c error.c hashovani.c execute.c
HEADERS = structs.h funkce.h helpers.h error.h hashovani.h execute.h

OBJ = ${SRC:.c=.o}

PARAMS = -std=c99 -g -Wall -pedantic
# -pg je pro gprof
# -g je pro gdb/valgrind

# optimalizace kompilace podle vysledku profielru
# http://www.abclinuxu.cz/blog/kazdy_den_jinak/2007/5/gcc-a-optimalizace-s-profilerem

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
	tar cjf ${NAME}-${VERSION}.tar.bz2 * .git

znova: clean all

tags: 
	ctags -R .
