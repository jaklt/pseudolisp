#include <stdlib.h>

#include "funkce.h"
#include "structs.h"
#include "error.h"
#include "execute.h"


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
		if (l->symbol->typ == LIST) {
			s = new_Symbol(LIST,
				doplnit_params(params, (List *)l->symbol->s.link));
		} else if (l->symbol->typ == PARAMETR) {
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


List *clone_List(List *l)
{
	if (l == NULL) return NULL;

	List *nl = new_List(l->symbol);
	List *nll = nl;

	while (l->next != NULL) {
		l = l->next;
		nll->next = new_List(l->symbol);
		nll = nll->next;
	}

	return nl;
}


Symbol *call(List *l)
{
	if (l == NULL) return NULL;
	if (l->symbol == NULL) return new_Symbol(LIST, l);

	Function *f;

	if (l->symbol->typ == THUNK) {
		f = ((Tank *)l->symbol->s.link)->function;
		// potencionalni leak, protoze zmizi link na l
		l = f_append( ((Tank *)l->next->symbol->s.link)->params, l);
	} else if (l->symbol->typ == FUNCTION) {
		f = (Function *)l->symbol->s.link;
		l = l->next;
	} else
		ERROR(VNITRNI_CHYBA);

	return new_Symbol_Tank(f, l);
}


Symbol *result(Function *f, List *params)
{
	if (f == NULL) return NULL;

	List *l = params;

	if (list_len(params) < f->number_of_params)
		return new_Symbol(THUNK, new_Tank(f, params));

	if (f->built_in == BUILT_IN)
		return f->body.link(params);


	// konvence je takova, ze prvni prvek listu vzdy odpovida bud:
	// - funkci/tanku = jde o volani function
	// - link na funkci list/NULL (nikoliv NIL) = jde o list
	l = doplnit_params(params, f->body.structure);

	if (l->symbol->typ == THUNK)
		l->symbol = resolve_Tank(l->symbol);

	return call(l);

	// nejsou vyreseny vnorene struktury:
	//
	// (a b c d (e f g) h i j) = pro e Tank/Funkci
}


Symbol *resolve_Tank(Symbol *s)
{
	if (s == NULL) return NULL;
	if (s->typ == LIST) s = call((List *)s->s.link);
	if (s == NULL || s->typ != THUNK) return s;


	Tank *t = (Tank *)s->s.link;

	while (t != NULL
		&& list_len(t->params) >= t->function->number_of_params)
	{
		s = result(t->function, t->params);
		t = (s != NULL && s->typ == THUNK) ? (Tank *)s->s.link : NULL;
	}

	return s;
}
