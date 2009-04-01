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
	if (is_NIL(s)) return NULL;
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


static int replace_Symbol(Symbol *s, Symbol *with)
{
	if (s == NULL) return 1;

	if (is_NIL(with)) {
		s->type = NIL;
		return 0;
	}

	s->type = with->type;
	s->s = with->s;
	return 0;
}


static Symbol *extend_Thunk(Symbol *s, List *l)
{
	if (is_NIL(s)) ERROR(VNITRNI_CHYBA);
	if (s->type != THUNK) {
		List *nl = new_List(s);
		nl->next = l;
		return new_Symbol(LIST, nl);
	}

	return new_Symbol(THUNK,
			new_Thunk(((Thunk *)s->s.link)->function,
				f_append(((Thunk *)s->s.link)->params, l)));
}


Symbol *result(Function *f, List *params)
{
	if (f == NULL) return NULL;
	if (f->built_in == BOOL_TRUE) return f->body.link(params);
	List *l = insert_params(params, f->body.structure);

	if (l == NULL || l->symbol == NULL) return new_Symbol(LIST, l);
	return resolve_Thunk(new_Symbol(LIST, l));
}


Symbol *resolve_Thunk(Symbol *s)
{
	if (is_NIL(s)) return NULL;
	Symbol *sp = s;
	Thunk *t;

	if (s->type == LIST) {
		List *l = (List *)s->s.link;
		l->symbol = resolve_Thunk(l->symbol);

		// TODO nemel by to bejt pred predchozim resolve_Thunk?
		if (is_NIL(l->symbol)) return new_Symbol(LIST, l);

		switch (l->symbol->type) {
			case THUNK:
				if (l->next == NULL) return resolve_Thunk(l->symbol);
				t = (Thunk *)l->symbol->s.link;

				// TODO zmenit na longer_or_append...
				if (list_len(t->params) >= t->function->params_count) {
					l->symbol = resolve_Thunk(l->symbol);
					return resolve_Thunk(s);
				}
				else {
					s = extend_Thunk(l->symbol, l->next);
					break;
				}
			case FUNCTION:
				s = new_Symbol(THUNK, new_Thunk((Function *)l->symbol->s.link, l->next));
				break;
			case LIST:
				l->symbol = resolve_Thunk(l->symbol);
				if (is_NIL(l->symbol)) {
					if  (l->next != NULL) ERROR(VNITRNI_CHYBA);
					return NULL;
				} else if (l->symbol->type == LIST) {
					if (l->next == NULL) return l->symbol;
					ERROR(VNITRNI_CHYBA);
				}
				return resolve_Thunk(s);
			default:
				if (l->next != NULL) ERROR(VNITRNI_CHYBA);
				return l->symbol;
		}
	}

	if (is_NIL(s) || s->type != THUNK) return s;
	t = (Thunk *)s->s.link;

	while (t != NULL && list_len(t->params) >= t->function->params_count)
	{
		s = result(t->function, t->params);
		if (!is_NIL(s) && s->type == LIST) s = resolve_Thunk(s);
		t = (!is_NIL(s) && s->type == THUNK) ? (Thunk *)s->s.link : NULL;
	}

	replace_Symbol(sp, s);
	return s;
}
