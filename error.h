#ifndef ERROR_H
#define ERROR_H

#define ERROR(n) {throw_full_error(n, __LINE__, __FILE__); return NULL; }


typedef enum {
	PRAZDNA_HODNOTA,
	OPERACE_NEMA_SMYSL,
	MOC_HLUBOKA_REKURZE,
	VNITRNI_CHYBA,
	NOT_IMPLEMENTED,
	UNDEFINED,
} E_ERROR;

int throw_error(E_ERROR t);
int throw_full_error(E_ERROR t, int line, char file[]);
// int throw_error(E_ERROR t, char *info);

#endif
