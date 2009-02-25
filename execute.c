#include <stdlib.h>

#include "structs.h"
#include "error.h"
#include "execute.h"


static List *f_append(List *a, List *b)
{
	List *ret = new_List(NULL);
	List *l = ret;

	while (a != NULL) {
		l->next = new_List(a->symbol);
		l = l->next;
		a = a->next;
	}

	while (b != NULL) {
		l->next = new_List(b->symbol);
		l = l->next;
		b = b->next;
	}

	l = ret->next; free(ret);
	return l;
}


static int list_len(List *l)
{
	int i = 0;
	while (l != NULL) {
		i++; l = l->next;
	}

	return i;
}


// TODO misto pro optimalizace prevedenim na pole
Symbol *get_parametr(List *params, int kolikaty)
{
	if (kolikaty < 1) return NULL;

	while (kolikaty > 1) {
		params = params->next;
		kolikaty--;
	}

	return params->symbol;
}


List *doplnit_params(List *params, List *kam)
{
	List *volani = new_List(NULL);
	List *vysledek = volani;
	List *l = kam;
	Symbol *s;

	while (l != NULL) {
		if (l->symbol->type == LIST) {
			s = new_Symbol(LIST,
				doplnit_params(params, (List *)l->symbol->s.link));
		} else if (l->symbol->type == PARAMETR) {
			s = get_parametr(params, l->symbol->s.character);
		} else {
			s = l->symbol;
		}

		volani->next = new_List(s);
		volani = volani->next;
		l = l->next;
	}

	l = vysledek->next;
	free(vysledek);
	return l; 
}


Symbol *call(List *l)
{
	if (l == NULL) return NULL;
	if (l->symbol == NULL) return new_Symbol(LIST, l);

	Function *f;

	if (l->symbol->type == THUNK) {
		f = ((Thunk *)l->symbol->s.link)->function;
	//	l = f_append( ((Thunk *)l->next->symbol->s.link)->params, l);
		List *params = ((Thunk *)l->symbol->s.link)->params;

		if (params == NULL)
			l = l->next;
		else if (l->next == NULL)
			l = params;
		else
			l = f_append(params , l->next);
	} else if (l->symbol->type == FUNCTION) {
		f = (Function *)l->symbol->s.link;
		l = l->next;
	} else if (l->symbol->type == LIST) {
		l->symbol = call((List *)l->symbol->s.link);
		return call(l); // XXX asi se zacykli
	} else
		ERROR(VNITRNI_CHYBA);
	

	return new_Symbol_Thunk(f, l);
}


Symbol *result(Function *f, List *params)
{
	if (f == NULL) return NULL;

	List *l = params;

	if (list_len(params) < f->number_of_params)
		return new_Symbol(THUNK, new_Thunk(f, params));

	if (f->built_in == BOOL_TRUE)
		return f->body.link(params);

	// konvence je takova, ze prvni prvek listu vzdy odpovida bud:
	// - funkci/tanku = jde o volani function
	// - link na funkci list/NULL (nikoliv NIL) = jde o list
	l = doplnit_params(params, f->body.structure);

	if (l->symbol->type == THUNK)
		l->symbol = resolve_Thunk(l->symbol);

	return call(l);
}


Symbol *resolve_Thunk(Symbol *s)
{
	if (s == NULL) return NULL;
	if (s->type == LIST) s = call((List *)s->s.link);
	if (s == NULL || s->type != THUNK) return s; // TODO is_NIL


	Thunk *t = (Thunk *)s->s.link;

	while (t != NULL
		&& list_len(t->params) >= t->function->number_of_params)
	{
		s = result(t->function, t->params);
		if (s != NULL && s->type == LIST) s = call((List *)s->s.link);
		t = (s != NULL && s->type == THUNK) ? (Thunk *)s->s.link : NULL;
	}

	return s;
}
