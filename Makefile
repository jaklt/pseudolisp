
VERSION = 0.0.0
NAME = pseudolisp

SRC = ${NAME}.c structs.c funkce.c helpers.c testy.c error.c
HEADERS = structs.h funkce.h helpers.h error.h

OBJ = ${SRC:.c=.o}

PARAMS = -std=c99 -g -Wall -pedantic


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
	tar cjf ${NAME}-${VERSION}.tar.bz2 *

znova: clean all
