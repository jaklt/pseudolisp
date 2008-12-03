#ifndef STRUCT_H
#define STRUCT_H

// TODO pridat t_cislo pro cisla

typedef enum {
	FUNKCE,
	SEZNAM,
	HODNOTA,
	TANK,
	PARAMETR
} E_TYP;


typedef enum {
	CISLO,
	BOOL,
	ZNAK,
} E_HODNOTA;


#define BUILT_IN      -1
#define NOT_BUILT_IN   0

typedef struct {
	E_TYP typ;
	int immortal : 1;
	void *odkaz;
} Symbol;


typedef struct SHodnota {
	E_HODNOTA typ;

	union {
		int cislo;
		unsigned int boolean : 1;
		char znak;
	} h;

} Hodnota;


typedef struct SList {
	Symbol *symbol;
	struct SList *dalsi;
} List;


typedef struct SFunkce {
	int built_in : 1;

	union {
		Symbol *(*odkaz)(List *);
		List *struktura;

	} telo;

	// struct SFunkce *vysledky;
	int pocet_parametru;
} Funkce;


typedef struct STank {
	Funkce *funkce;
	List *parametry;
} Tank;


Funkce *get_Funkce(char *jmeno);
Funkce *new_Funkce(List *telo_funkce, int pocet_parametru);
Hodnota *new_Hodnota(E_HODNOTA typ, int co);

List *new_List(Symbol *symbol);
List *array_to_List(Symbol **seznam_symbolu, int pocet_symbolu);

/**
 * Konstruktor symbolu.
 */
Symbol *new_Symbol(E_TYP typ, void *symbol);


/**
 * Vytvori novy Tank, ktery v pripade nutnosti znalosti hodnoty slouzi k
 * zavolani patricne funkce s danymi parametry
 */
Tank *new_Tank(Funkce *fce, List *parametry);


#endif
