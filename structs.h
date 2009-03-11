#ifndef STRUCT_H
#define STRUCT_H

typedef double t_number;

typedef enum {
	NIL,
	FUNCTION,
	LIST,
	NUMBER,
	BOOL,
	CHAR,
	THUNK,
	PARAMETER,
} E_TYPE;

#define BOOL_TRUE  1
#define BOOL_FALSE 0


typedef struct {
	E_TYPE type;

	union {
		t_number number;
		unsigned int boolean : 1;
		char character;
		void *link;
	} s;
} Symbol;


typedef struct SList {
	Symbol *symbol;
	struct SList *next;
} List;


typedef struct SFunction {
	unsigned int built_in : 1;
	int params_count;

	union {
		Symbol *(*link)(List *);
		List *structure;
	} body;
} Function;


typedef struct SThunk {
	Function *function;
	List *params;
} Thunk;


Function *get_Function(char *name);
Function *new_Function(List *body_function, int params_count);

List *new_List(Symbol *symbol);


/**
 * Konstruktor symbolu.
 */
Symbol *new_Symbol(E_TYPE type, void *symbol);

Symbol *new_Ordinal(E_TYPE type, double co);
Symbol *new_NIL();
int is_NIL(Symbol *s);

/**
 * Vytvori novy Thunk, ktery v pripade nutnosti znalosti hodnoty slouzi k
 * zavolani patricne function s danymi params
 */
Thunk *new_Thunk(Function *fce, List *params);


#endif
