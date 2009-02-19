#include <stdio.h>
#include "error.h"

#define ERR_START " !!! Error: "
#define ERR_END " !!!\n"


int throw_full_error(E_ERROR t, int line, char file[])
{
	if (line) printf(ERR_START "on %i. line in %s file:\n", line, file);

	printf(ERR_START);

	switch (t) {
		case PRAZDNA_HODNOTA:
			printf("Hodnota je prazdna");
			break;
		case OPERACE_NEMA_SMYSL:
			printf("Operace nad neslucitelnymi typy");
			break;
		case MOC_HLUBOKA_REKURZE:
			printf("Prekonan maximalni hloubka rekurze (&cislo)");
			break;
		case VNITRNI_CHYBA:
			printf("Bad Function body");
			break;
		case NOT_IMPLEMENTED:
			printf("Not implemented yet");
			break;
		case UNDEFINED:
			printf("Undefined");
			break;
		case SYNTAX_ERROR:
			printf("Syntax error");
			break;
		default:
			printf("Unknown error\n");
			break;
	}

	printf(ERR_END);
	return 0;
}


int throw_error(E_ERROR t)
{
	throw_full_error(t, 0, NULL);
	return 0;
}
