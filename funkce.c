#include <stdlib.h>

#include "funkce.h"
#include "structs.h"
#include "error.h"
#include "execute.h"


static Symbol *inner_reduce(
		Symbol *(),
		Symbol *(*overeni)(Symbol *(), Symbol *, Symbol *),
		List *l
	);


Function **get_array_of_funtions()
{
	// - vytvoreni instanci vsech built-in funkci

	#define n 7
	static Function *array[n];
	static int hotovson = 1;
	
	if (hotovson) {
		hotovson = 0;

		array[0] = new_Function(NULL, 2);
		array[0]->built_in = BUILT_IN;
		array[0]->body.link = plus;

		array[1] = new_Function(NULL, 2);
		array[1]->built_in = BUILT_IN;
		array[1]->body.link = minus;

		array[2] = new_Function(NULL, 2);
		array[2]->built_in = BUILT_IN;
		array[2]->body.link = krat;

		array[3] = new_Function(NULL, 2);
		array[3]->built_in = BUILT_IN;
		array[3]->body.link = deleno;

		array[4] = new_Function(NULL, 3);
		array[4]->built_in = BUILT_IN;
		array[4]->body.link = op_if;

		array[5] = new_Function(NULL, 2);
		array[5]->built_in = BUILT_IN;
		array[5]->body.link = eq;

		array[6] = new_Function(NULL, 2);
		array[6]->built_in = BUILT_IN;
		array[6]->body.link = gt;
	}

	return array;
}


int list_len(List *l)
{
	int i = 0;
	while (l != NULL) {
		i++; l = l->next;
	}

	return i;
}


static int is_NIL(Symbol *s)
{
	return (s == NULL || s->typ == NIL);
}


static List *get_List(Symbol *s)
{
	if (is_NIL(s) || s->typ != LIST
			|| !is_NIL(((List *)s->s.link)->symbol))
	{
		return NULL;
	} else {
		List *l = ((List *)s->s.link)->next;
		return l == NULL ? new_List(new_NIL()) : l;
	}
}


/**
 * Typicke funkcionalni funkce
 * ---------------------------
 */

Symbol *map(List *l)
{
	return NULL;
}


Symbol *reduce(List *l)
{
	// volani asi result(f, nl);
	return NULL;
}


Symbol *filter(List *l)
{
	return NULL;
}


/**
 * Operace se seznamy
 * ------------------
 */

Symbol *head(List *l)
{
	if (l == NULL) return NULL;

	return l->symbol;
}


Symbol *tail(List *l)
{
	if (l == NULL) return NULL;

	List *ret = get_List(l->symbol);
	
	if (ret == NULL)
		return list(l->next);

	return list(ret->next);
}


Symbol *list(List *params)
{
	List *l = new_List(NULL);
	l->next = params;

	return new_Symbol(LIST, l);
}


List *f_append(List *a, List *b)
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

	return ret;
}


static inline Symbol *f_append_operace(List *a, List *b)
{
	return new_Symbol(LIST, f_append(a, b));
}


Symbol *ok_listy(Symbol *(*operace)(List *, List *), Symbol *a, Symbol *b)
{
	if (a == NULL || b == NULL) ERROR(PRAZDNA_HODNOTA);

	List *la, *lb;

	if ((la = get_List(a)) == NULL) la = new_List(a);
	if ((lb = get_List(b)) == NULL) lb = new_List(b);

	return operace(la, lb);
}


Symbol *append(List *params)
{
	return inner_reduce(f_append_operace, ok_listy, params);
}


Symbol *take(List *params)
{
	return NULL;
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
	if (a == NULL || b == NULL) ERROR(PRAZDNA_HODNOTA);
	if (a->typ != NUMBER || b->typ != NUMBER) ERROR(OPERACE_NEMA_SMYSL);

	return operace(a->s.number, b->s.number);
}


/**
 * Function pro porovnavani
 * ----------------------
 */

static inline Symbol *is_not_null(Symbol *vysl)
{
	if (vysl != NULL && vysl->typ != NIL)
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
	Symbol *s = resolve_Tank(params->symbol);

	if (s->typ != BOOL) ERROR(OPERACE_NEMA_SMYSL);

	if (s->s.boolean)
		return params->next->symbol;
	else
		return params->next->next->symbol;
}


Symbol *op_and(List *params)
{
	return NULL;
}


Symbol *op_or (List *params)
{
	return NULL;
}


Symbol *op_not(List *params)
{
	return NULL;
}


/**
 * next pomocne funkce
 * --------------------
 */

Symbol *print(List *params)
{
	return NULL;
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
		// TODO resolve_Tank zde je brzo ne?
		s = overeni(operace, resolve_Tank(s), resolve_Tank(l->symbol));
		l= l->next;
	}

	return s;
}
