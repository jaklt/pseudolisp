#include <stdlib.h>
#include "structs.h"



Function *new_Function(List *body_function, int number_of_params)
{
	Function *f = (Function *) malloc(sizeof(Function));

	f->body.structure = body_function;
	f->number_of_params = number_of_params;
	f->built_in = NOT_BUILT_IN;

	return f;
}


List *new_List(Symbol *symbol)
{
	List *l = (List *) malloc(sizeof(List));

	l->symbol = symbol;
	l->next = NULL;

	return l;
}


List *array_to_List(Symbol **seznam_symbolu, int pocet_symbolu)
{
	if (pocet_symbolu <= 0) return NULL;

	List *l = (List *) malloc(pocet_symbolu * sizeof(List));
	List *ret = l;

	for (int i=0; i<pocet_symbolu; i++) {
		l->symbol = (Symbol *) seznam_symbolu[i];
		l->next = NULL;
		
		if (i != 0) (l-1)->next = l;
		l++;
	}

	return ret;
}


Symbol *new_Ordinal(E_TYP typ, t_number co)
{
	Symbol *s = (Symbol *) malloc(sizeof(Symbol));

	s->typ = typ;

	switch (typ) {
		case NUMBER: s->s.number  =        co; break;
		case   BOOL: s->s.boolean = (int)  co; break;
		case PARAMETR:
		case  CHAR: s->s.character    = (char) co; break;
		default: break;
	}

	return s;
}


Symbol *new_Symbol(E_TYP typ, void *symbol)
{
	Symbol *s = (Symbol *) malloc(sizeof(Symbol));

	s->typ = typ;
	s->s.link = (void *)symbol;

	return s;
}


Symbol *new_NIL() { return new_Symbol(NIL, NULL); }


Tank *new_Tank(Function *fce, List *params)
{
	Tank *t = (Tank *) malloc(sizeof(Tank));

	t->function = fce;
	t->params = params;

	return t;
}


Symbol *new_Symbol_List(Symbol *symbol)
{
	return new_Symbol(LIST, new_List(symbol));
}


Symbol *new_Symbol_Function(List *function_body, int pocet_symbolu)
{
	return new_Symbol(FUNCTION, new_Function(function_body, pocet_symbolu));
}


Symbol *new_Symbol_Tank(Function *fce, List *params)
{
	return new_Symbol(THUNK, new_Tank(fce, params));
}
