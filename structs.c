#include <stdlib.h>
#include "structs.h"


Function *new_Function(List *body_function, int number_of_params)
{
	Function *f = (Function *) malloc(sizeof(Function));

	f->body.structure = body_function;
	f->number_of_params = number_of_params;
	f->built_in = BOOL_FALSE;

	return f;
}


List *new_List(Symbol *symbol)
{
	List *l = (List *) malloc(sizeof(List));

	l->symbol = symbol;
	l->next = NULL;

	return l;
}


Symbol *new_Ordinal(E_TYPE type, t_number co)
{
	Symbol *s = (Symbol *) malloc(sizeof(Symbol));

	s->type = type;

	switch (type) {
		case NUMBER: s->s.number  =        co; break;
		case   BOOL: s->s.boolean = (int)  co; break;
		case PARAMETR:
		case  CHAR: s->s.character    = (char) co; break;
		default: break;
	}

	return s;
}


Symbol *new_Symbol(E_TYPE type, void *symbol)
{
	Symbol *s = (Symbol *) malloc(sizeof(Symbol));

	s->type = type;
	s->s.link = (void *)symbol;

	return s;
}


Symbol *new_NIL() { return new_Symbol(NIL, NULL); }


Thunk *new_Thunk(Function *fce, List *params)
{
	Thunk *t = (Thunk *) malloc(sizeof(Thunk));

	t->function = fce;
	t->params = params;

	return t;
}
