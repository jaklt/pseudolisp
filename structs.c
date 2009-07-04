#include <stdlib.h>
#include "structs.h"
#include "gc.h"


Function *new_Function(List *body_function, int params_count)
{
	Function *f = (Function *) malloc(sizeof(Function));

	f->body.structure = body_function;
	f->params_count   = params_count;
	f->built_in       = BOOL_FALSE;
	f->more_params    = BOOL_FALSE;

	gc_collect(FUNCTION, f);
	return f;
}


List *new_List(Symbol *symbol)
{
	List *l = (List *) malloc(sizeof(List));

	l->symbol = symbol;
	l->next = NULL;

	gc_collect(LIST, l);
	return l;
}


Symbol *new_Ordinal(E_TYPE type, t_number co)
{
	Symbol *s = (Symbol *) malloc(sizeof(Symbol));

	s->type = type;

	switch (type) {
		case PARAMETER:
		case NUMBER: s->s.number   =        co; break;
		case   BOOL: s->s.boolean  = (int)  co; break;
		case  CHAR: s->s.character = (char) co; break;
		default: break;
	}

	gc_collect(NIL, s);
	return s;
}


Symbol *new_Symbol(E_TYPE type, void *symbol)
{
	Symbol *s = (Symbol *) malloc(sizeof(Symbol));

	s->type = type;
	s->s.link = (void *)symbol;

	gc_collect(NIL, s);
	return s;
}


Symbol *new_NIL()
{
	// TODO only one instance (in cooperation with gc)
	Symbol *s = new_Symbol(NIL, NULL);
	gc_collect(NIL, s);
	return s;
}


int is_NIL(Symbol *s)
{
	if (s == NULL || s->type == NIL) return 1;

	// empty List
	if (s->type == LIST) {
		List *l = (List *) s->s.link;
		return (is_NIL(l->symbol) && l->next == NULL);
	}

	return 0;
}


Thunk *new_Thunk(Function *fce, List *params)
{
	Thunk *t = (Thunk *) malloc(sizeof(Thunk));

	t->function = fce;
	t->params = params;

	gc_collect(THUNK, t);
	return t;
}
