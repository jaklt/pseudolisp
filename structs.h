#ifndef STRUCT_H
#define STRUCT_H

typedef double t_number;

typedef enum {
	FUNCTION,
	LIST,
	NIL,
	NUMBER,
	BOOL,
	CHAR,
	THUNK,
	PARAMETR,
} E_TYP;

#define BOOL_TRUE -1
#define BOOL_FALSE 0

#define BUILT_IN      -1
#define NOT_BUILT_IN   0

typedef struct {
	E_TYP typ;

	union {
		t_number number;
		int boolean : 1;
		char character;
		void *link;
	} s;
} Symbol;


typedef struct SList {
	Symbol *symbol;
	struct SList *next;
} List;


typedef struct SFunction {
	int built_in : 1;
	int number_of_params;

	union {
		Symbol *(*link)(List *);
		List *structure;

	} body;
} Function;


typedef struct STank {
	Function *function;
	List *params;
} Tank;


Function *get_Function(char *jmeno);
Function *new_Function(List *body_function, int number_of_params);

List *new_List(Symbol *symbol);
List *array_to_List(Symbol **seznam_symbolu, int pocet_symbolu);

/**
 * Konstruktor symbolu.
 */
Symbol *new_Symbol(E_TYP typ, void *symbol);

Symbol *new_Symbol_List(Symbol *symbol);
Symbol *new_Symbol_Function(List *function_body, int pocet_symbolu);
Symbol *new_Symbol_Tank(Function *fce, List *params);

Symbol *new_Ordinal(E_TYP typ, double co);
Symbol *new_NIL();

/**
 * Vytvori novy Tank, ktery v pripade nutnosti znalosti hodnoty slouzi k
 * zavolani patricne function s danymi params
 */
Tank *new_Tank(Function *fce, List *params);


int uvolnit(E_TYP, void *co); // ???

void *clone(E_TYP, void *co); // ???

#endif
