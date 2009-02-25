#ifndef ERROR_H
#define ERROR_H

// #define __MAKE_ERROR(n) {throw_full_error(n, __LINE__, __FILE__); }
#define __MAKE_ERROR(n) {throw_error(n); }

#define ERROR(n)  {__MAKE_ERROR(n); return NULL; }
#define iERROR(n) {__MAKE_ERROR(n); return 0; }


typedef enum {
	PRAZDNA_HODNOTA,
	OPERACE_NEMA_SMYSL,
	MOC_HLUBOKA_REKURZE,
	VNITRNI_CHYBA,
	NOT_IMPLEMENTED,
	UNDEFINED,
	SYNTAX_ERROR,
} E_ERROR;

int throw_error(E_ERROR t);
int throw_full_error(E_ERROR t, int line, char file[]);

#endif
