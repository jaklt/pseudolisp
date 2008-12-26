#include <stdio.h>
#include "error.h"

#define ERR_START " !!! Chyba: "
#define ERR_END " !!!\n"


int throw_error(E_ERROR t)
{
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
			printf("Spatne sestavene telo funkce");
			break;
		default:
			printf(ERR_START "Neznama chyba\n");
			break;
	}

	printf(ERR_END);
	return 0;
}
