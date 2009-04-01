#include <stdlib.h>

#include "funkce.h"
#include "error.h"
#include "execute.h"


static Symbol *inner_reduce(
		Symbol *(),
		Symbol *(*overeni)(Symbol *(), Symbol *, Symbol *),
		List *l
	);


int is_TRUE(Symbol *s)
{
	return (!is_NIL(s) && s->type == BOOL && s->s.boolean);
}


int Symbol_to_bool(Symbol *symb)
{
	Symbol *s = resolve_Thunk(symb);
	if (!is_NIL(s) && s->type != BOOL) iERROR(OPERACE_NEMA_SMYSL);

	return is_TRUE(s);
}


static List *get_List(Symbol *s)
{
	if (is_NIL(s) || s->type != LIST
			|| !is_NIL(((List *)s->s.link)->symbol))
	{
		return NULL;
	} else {
		return ((List *)s->s.link)->next;
	}
}


/**
 * Operace se seznamy
 * ------------------
 */

Symbol *head(List *l)
{
	List *hl = get_List(resolve_Thunk(l->symbol));

	if (hl == NULL) ERROR(OPERACE_NEMA_SMYSL);
	return hl->symbol;
}


Symbol *tail(List *l)
{
	List *ret = get_List(resolve_Thunk(l->symbol));
	
	if (ret == NULL) ERROR(OPERACE_NEMA_SMYSL);
	return ret->next ? list(ret->next) : NULL;
}


Symbol *list(List *params)
{
	List *l = new_List(NULL);
	l->next = params;

	return new_Symbol(LIST, l);
}


static int is_Empty(Symbol *s)
{
	if (is_NIL(s)) return 1;

	if (s->type == LIST) {
		List *l = (List *)s->s.link;
		return (is_NIL(l->symbol) && l->next == NULL);
	}

	return 0;
}


Symbol *append(List *params)
{
	List *vysl = new_List(NULL);
	List *l = vysl;
	List *cp;
	Symbol *s;

	while (params != NULL) {
		s = resolve_Thunk(params->symbol);
		if (is_Empty(s)) { params = params->next; continue; };
	
		cp = get_List(s);

		// nejde o list -> pripojit jak to je
		if (cp == NULL) {
			l->next = new_List(s);
			l = l->next;
		}

		// kopirovani cp pokud se jedna o List
		while (cp != NULL) {
			l->next = new_List(cp->symbol);
			l = l->next;
			cp = cp->next;
		}

		params = params->next;
	}

	return new_Symbol(LIST, vysl);
}


/**
 * Aritmeticke operace s cisly
 * ---------------------------
 */

Symbol *nubers_ok(Symbol *(*operace)(t_number, t_number), Symbol *a, Symbol *b);


static inline Symbol *f_plus  (t_number a, t_number b) { return new_Ordinal(NUMBER, a+b); }
static inline Symbol *f_krat  (t_number a, t_number b) { return new_Ordinal(NUMBER, a*b); }
static inline Symbol *f_minus (t_number a, t_number b) { return new_Ordinal(NUMBER, a-b); }
static inline Symbol *f_deleno(t_number a, t_number b) { return new_Ordinal(NUMBER, a/b); }


Symbol *plus  (List *params) { return inner_reduce(f_plus,   nubers_ok, params); }
Symbol *krat  (List *params) { return inner_reduce(f_krat,   nubers_ok, params); }
Symbol *minus (List *params) { return inner_reduce(f_minus,  nubers_ok, params); }
Symbol *deleno(List *params) { return inner_reduce(f_deleno, nubers_ok, params); }


Symbol *nubers_ok(Symbol *(*operace)(t_number, t_number), Symbol *a, Symbol *b)
{
//	a = resolve_Thunk(a);
//	b = resolve_Thunk(b);

	if (a == NULL || b == NULL) ERROR(PRAZDNA_HODNOTA);
	if (a->type != NUMBER || b->type != NUMBER) ERROR(OPERACE_NEMA_SMYSL);

	return operace(a->s.number, b->s.number);
}


/**
 * Function pro porovnavani
 * ----------------------
 */

static inline Symbol *is_not_null(Symbol *vysl)
{
	if (!is_NIL(vysl))
		return new_Ordinal(BOOL, BOOL_TRUE);
	else
		return new_Ordinal(BOOL, BOOL_FALSE);
}


static inline Symbol *f_eq(t_number a, t_number b)
{
	return (a == b) ? new_Ordinal(NUMBER, a) : new_NIL(); 
}


static inline Symbol *f_gt(t_number a, t_number b)
{
	return (a > b) ? new_Ordinal(NUMBER, b) : new_NIL();
}


Symbol *eq(List *params)
{
	return is_not_null(inner_reduce(f_eq, nubers_ok, params));
}


Symbol *gt(List *params)
{
	return is_not_null(inner_reduce(f_gt, nubers_ok, params));
}


Symbol *op_if(List *params)
{
	Symbol *s = resolve_Thunk(params->symbol);

	if (s->type != BOOL) ERROR(OPERACE_NEMA_SMYSL);

	if (s->s.boolean)
		return params->next->symbol;
	else
		return params->next->next->symbol;
}


Symbol *op_and(List *params)
{
	int t = 1;

	while (params != NULL && t) {
		t = Symbol_to_bool(params->symbol);
		params = params->next;
	}

	return new_Ordinal(BOOL, t);
}


Symbol *op_or (List *params)
{
	int t = 0;

	while (params != NULL && !t) {
		t = Symbol_to_bool(params->symbol);
		params = params->next;
	}

	return new_Ordinal(BOOL, t);
}


Symbol *op_not(List *params)
{
	Symbol *s = resolve_Thunk(params->symbol);

	if (is_NIL(s) || s->type != BOOL)
		ERROR(OPERACE_NEMA_SMYSL);

	return new_Ordinal(BOOL, s->s.boolean ? BOOL_FALSE : BOOL_TRUE);
}


Symbol *op_nil(List *params)
{
	return new_Ordinal(BOOL, is_NIL(resolve_Thunk(params->symbol)));
}


Symbol *op_list(List *params)
{
	return new_Ordinal(BOOL,
			get_List(resolve_Thunk(params->symbol)) != NULL ? BOOL_TRUE : BOOL_FALSE);
}


/**
 * next pomocne funkce
 * --------------------
 */

Symbol *undefined(List *params)
{
	ERROR(UNDEFINED);
}


static Symbol *inner_reduce(
		Symbol *(*operace)(void),
		Symbol *(*overeni)(Symbol *(*operace)(void), Symbol *, Symbol *),
		List *l
	)
{
	if (l == NULL) ERROR(VNITRNI_CHYBA);

	Symbol *s = l->symbol;
	l = l->next;

	while (l != NULL && s != NULL) {
		// TODO ma byt kontrola na NULL nebo NIL?
		// TODO resolve_Thunk zde je brzo ne?
		s = overeni(operace, resolve_Thunk(s), resolve_Thunk(l->symbol));
		l= l->next;
	}

	return s;
}
