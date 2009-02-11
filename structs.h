#ifndef STRUCT_H
#define STRUCT_H

typedef double t_cislo;

typedef enum {
	FUNKCE,
	LIST,
	NIL,
	CISLO,
	BOOL,
	ZNAK,
	TANK, // alias Thunk ;-)
	PARAMETR,
} E_TYP;

#define BOOL_TRUE -1
#define BOOL_FALSE 0

#define BUILT_IN      -1
#define NOT_BUILT_IN   0

typedef struct {
	E_TYP typ;
	int immortal : 1;

	union {
		t_cislo cislo;
		int boolean : 1;
		char znak;
		void *odkaz;
	} s;
} Symbol;


typedef struct SList {
	Symbol *symbol;
	struct SList *dalsi;
} List;


typedef struct SFunkce {
	int built_in : 1;
	int pocet_parametru;

	union {
		Symbol *(*odkaz)(List *);
		List *struktura;

	} telo;
} Funkce;


typedef struct STank {
	Funkce *funkce;
	List *parametry;
} Tank;


Funkce *get_Funkce(char *jmeno);
Funkce *new_Funkce(List *telo_funkce, int pocet_parametru);

List *new_List(Symbol *symbol);
List *array_to_List(Symbol **seznam_symbolu, int pocet_symbolu);

/**
 * Konstruktor symbolu.
 */
Symbol *new_Symbol(E_TYP typ, void *symbol);

Symbol *new_Symbol_List(Symbol *symbol);
Symbol *new_Symbol_Funkce(List *telo_funkce, int pocet_symbolu);
Symbol *new_Symbol_Tank(Funkce *fce, List *parametry);

Symbol *new_Ordinal(E_TYP typ, double co);
Symbol *new_NIL();

/**
 * Vytvori novy Tank, ktery v pripade nutnosti znalosti hodnoty slouzi k
 * zavolani patricne funkce s danymi parametry
 */
Tank *new_Tank(Funkce *fce, List *parametry);


int uvolnit(E_TYP, void *co); // ???

void *clone(E_TYP, void *co); // ???

#endif
