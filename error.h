#ifndef ERROR_H
#define ERROR_H

#define ERROR(n) {throw_error(n); return NULL; }


typedef enum {
	PRAZDNA_HODNOTA,
	OPERACE_NEMA_SMYSL,
	MOC_HLUBOKA_REKURZE,
} E_ERROR;

int throw_error(E_ERROR t);
// int throw_error(E_ERROR t, char *info);

#endif
