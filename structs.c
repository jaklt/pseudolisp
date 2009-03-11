#include <stdlib.h>
#include "structs.h"


Function *new_Function(List *body_function, int params_count)
{
	Function *f = (Function *) malloc(sizeof(Function));

	f->body.structure = body_function;
	f->params_count = params_count;
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
		case PARAMETER:
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


int is_NIL(Symbol *s)
{
	return (s == NULL || s->type == NIL);
}


Thunk *new_Thunk(Function *fce, List *params)
{
	Thunk *t = (Thunk *) malloc(sizeof(Thunk));

	t->function = fce;
	t->params = params;

	return t;
}
