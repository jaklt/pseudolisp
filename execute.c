#include <stdlib.h>

#include "error.h"
#include "execute.h"


static List *f_append(List *a, List *b)
{
	if (a == NULL) return b;
	List *ret = new_List(NULL);
	List *l = ret;

	while (a != NULL) {
		l->next = new_List(a->symbol);
		l = l->next;
		a = a->next;
	}

	l->next = b;
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


static List *insert_params_run(Symbol **exp_params, List *kam);
static Symbol *insert_params_solve(Symbol **exp_params, Symbol *s)
{
	if (s->type == LIST)
		return new_Symbol(LIST, insert_params_run(exp_params, (List *) s->s.link));
	else if (s->type == PARAMETER)
		return exp_params[s->s.character - 1];

	return s;
}


static List *insert_params_run(Symbol **exp_params, List *l)
{
	if (l == NULL) return NULL;
	List *ret = new_List(insert_params_solve(exp_params, l->symbol));
	List *ret_tmp = ret;

	while (l->next != NULL) {
		l = l->next;
		ret->next = new_List(insert_params_solve(exp_params, l->symbol));
		ret = ret->next;
	}

	return ret_tmp;
}


List *insert_params(List *params, List *kam)
{
	Symbol **exp_params = (Symbol **) malloc(list_len(params) * sizeof(Symbol *));
	List *l;

	for (int i=0; params != NULL; i++) {
		exp_params[i] = params->symbol;
		params = params->next;
	}

	l = insert_params_run(exp_params, kam);

	free(exp_params);
	return l;
}


Symbol *call(List *l)
{
	if (l == NULL) return NULL;
	if (l->symbol == NULL) return new_Symbol(LIST, l);

	Function *f;

	if (l->symbol->type == THUNK) {
		if (l->next == NULL) return l->symbol;

		f = ((Thunk *)l->symbol->s.link)->function;
		l = f_append( ((Thunk *)l->symbol->s.link)->params, l->next);
	} else if (l->symbol->type == FUNCTION) {
		f = (Function *)l->symbol->s.link;
		l = l->next;
	} else if (l->symbol->type == LIST) {
		l->symbol = call((List *)l->symbol->s.link);
		return call(l);
	} else {
		if (l->next == NULL) return l->symbol;
		ERROR(VNITRNI_CHYBA);
	}

	return new_Symbol(THUNK, new_Thunk(f, l));
}


Symbol *result(Function *f, List *params)
{
	if (f == NULL) return NULL;

	List *l = params;

	if (list_len(params) < f->params_count)
		return new_Symbol(THUNK, new_Thunk(f, params));

	if (f->built_in == BOOL_TRUE)
		return f->body.link(params);

	// konvence je takova, ze prvni prvek listu vzdy odpovida bud:
	// - funkci/tanku = jde o volani function
	// - link na funkci list/NULL (nikoliv NIL) = jde o list
	l = insert_params(params, f->body.structure);

	if (l->symbol->type == THUNK)
		l->symbol = resolve_Thunk(l->symbol);

	return call(l);
}


Symbol *resolve_Thunk(Symbol *s)
{
	if (s == NULL) return NULL;
	if (s->type == LIST) s = call((List *)s->s.link);
	if (is_NIL(s) || s->type != THUNK) return s;

	Thunk *t = (Thunk *)s->s.link;

	while (t != NULL
		&& list_len(t->params) >= t->function->params_count)
	{
		s = result(t->function, t->params);
		if (s != NULL && s->type == LIST) s = call((List *)s->s.link);
		t = (s != NULL && s->type == THUNK) ? (Thunk *)s->s.link : NULL;
	}

	return s;
}
