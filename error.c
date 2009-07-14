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
		case PRAZDNA_HODNOTA:
			fprintf(stderr, "Empty value");
			break;
		case OPERACE_NEMA_SMYSL:
			fprintf(stderr, "Operace nad neslucitelnymi typy");
			break;
		case MOC_HLUBOKA_REKURZE:
			fprintf(stderr, "Prekonan maximalni hloubka rekurze (&cislo)");
			break;
		case VNITRNI_CHYBA:
			fprintf(stderr, "Bad Function body");
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
	exit(t + 1);
	return 1;
}


inline int throw_error(E_ERROR t)
{
	return throw_full_error(t, 0, NULL);
}
