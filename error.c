#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define ERR_START " !!! Error: "
#define ERR_END " !!!\n"


int throw_full_error(E_ERROR t, int line, char file[])
{
	if (line) fprintf(stderr, ERR_START "on %i. line in %s file:\n", line, file);

	fprintf(stderr, ERR_START);

	switch (t) {
		case TYPE_ERROR:
			fprintf(stderr, "Type error");
			break;
		case TOO_DEEP_RECURSION:
			fprintf(stderr, "Too deep recursion (&number)");
			break;
		case TOO_MANY_PARAMS:
			fprintf(stderr, "Too many parameters");
			break;
		case INNER_ERROR:
			fprintf(stderr, "Inner error (maybe bad function body)");
			break;
		case NOT_IMPLEMENTED:
			fprintf(stderr, "Not implemented yet");
			break;
		case UNDEFINED:
			fprintf(stderr, "Undefined");
			break;
		case SYNTAX_ERROR:
			fprintf(stderr, "Syntax error");
			break;
		default:
			fprintf(stderr, "Unknown error");
			break;
	}

	fprintf(stderr, ERR_END);
#ifdef DEBUG
	(* (int *) 0) = 0;
#else
	exit(t + 1);
#endif
	return t + 1;
}


inline int throw_error(E_ERROR t)
{
	return throw_full_error(t, 0, NULL);
}
