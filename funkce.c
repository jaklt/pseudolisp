#include <stdlib.h>

#include "funkce.h"
#include "structs.h"
#include "error.h"
#include "execute.h"


static Symbol *vnitrni_reduce(
		Symbol *(),
		Symbol *(*overeni)(Symbol *(), Symbol *, Symbol *),
		List *l
	);


Funkce **get_array_of_funtions()
{
	// - vytvoreni instanci vsech built-in funkci

	#define n 7
	static Funkce *array[n];
	static int hotovson = 1;
	
	if (hotovson) {
		hotovson = 0;

		array[0] = new_Funkce(NULL, 2);
		array[0]->built_in = BUILT_IN;
		array[0]->telo.odkaz = plus;

		array[1] = new_Funkce(NULL, 2);
		array[1]->built_in = BUILT_IN;
		array[1]->telo.odkaz = minus;

		array[2] = new_Funkce(NULL, 2);
		array[2]->built_in = BUILT_IN;
		array[2]->telo.odkaz = krat;

		array[3] = new_Funkce(NULL, 2);
		array[3]->built_in = BUILT_IN;
		array[3]->telo.odkaz = deleno;

		array[4] = new_Funkce(NULL, 3);
		array[4]->built_in = BUILT_IN;
		array[4]->telo.odkaz = op_if;

		array[5] = new_Funkce(NULL, 2);
		array[5]->built_in = BUILT_IN;
		array[5]->telo.odkaz = eq;

		array[6] = new_Funkce(NULL, 2);
		array[6]->built_in = BUILT_IN;
		array[6]->telo.odkaz = gt;
	}

	return array;
}


int delka_listu(List *l)
{
	int i = 0;
	while (l != NULL) {
		i++; l = l->dalsi;
	}

	return i;
}


List *posledni_z_listu(List *l)
{
	if (l == NULL) return NULL;

	while (l->dalsi != NULL) l = l->dalsi;

	return l;
}


/**
 * Typicke funkcionalni funkce
 * ---------------------------
 */

Symbol *reduce(Funkce *f, List *l)
{
	if (l == NULL || f == NULL) return NULL;

	List *nl = new_List(l->symbol);
	nl->dalsi = new_List(NULL);
	l = l->dalsi;

	while (l != NULL) {	
		if (l->symbol->typ == TANK) result(NULL, NULL); // XXX
		nl->dalsi->symbol = l->symbol;
		nl->symbol = result(f, nl);

		l= l->dalsi;
	}

	return nl->symbol;
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
	if (l == NULL || l->dalsi == NULL)
		return NULL;

	return list(l->dalsi);
}


Symbol *list(List *parametry)
{
	List *l = new_List(NULL);
	l->dalsi = parametry;

	return new_Symbol(LIST, l);
}


List *f_append(List *a, List *b)
{
	List *ret = new_List(NULL);
	List *l = ret;

	while (a != NULL) {
		l->dalsi = new_List(a->symbol);
		l = l->dalsi;
		a = a->dalsi;
	}

	while (b != NULL) {
		l->dalsi = new_List(b->symbol);
		l = l->dalsi;
		b = b->dalsi;
	}

	return ret;
}


static inline Symbol *f_append_operace(List *a, List *b)
{
	return new_Symbol(LIST, f_append(a, b));
}


static List *get_List(Symbol *s)
{
	if (s->typ != LIST || ((List *)s->s.odkaz)->symbol != NULL)
		return NULL;
	else {
		List *l = ((List *)s->s.odkaz)->dalsi;
		return l == NULL ? new_List(new_NIL()) : l;
	}
}


Symbol *ok_listy(Symbol *(*operace)(List *, List *), Symbol *a, Symbol *b)
{
	if (a == NULL || b == NULL) ERROR(PRAZDNA_HODNOTA);

	List *la, *lb;

	if ((la = get_List(a)) == NULL) la = new_List(a);
	if ((lb = get_List(b)) == NULL) lb = new_List(b);

	return operace(la, lb);
}


Symbol *append(List *parametry)
{
	return vnitrni_reduce(f_append_operace, ok_listy, parametry);
}


Symbol *take(List *parametry)
{
	return NULL;
}


/**
 * Aritmeticke operace s cisly
 * ---------------------------
 */

Symbol *ok_cisla(Symbol *(*operace)(t_cislo, t_cislo), Symbol *a, Symbol *b);


static inline Symbol *f_plus  (t_cislo a, t_cislo b) { return new_Ordinal(CISLO, a+b); }
static inline Symbol *f_krat  (t_cislo a, t_cislo b) { return new_Ordinal(CISLO, a*b); }
static inline Symbol *f_minus (t_cislo a, t_cislo b) { return new_Ordinal(CISLO, a-b); }
static inline Symbol *f_deleno(t_cislo a, t_cislo b) { return new_Ordinal(CISLO, a/b); }


Symbol *plus  (List *parametry) { return vnitrni_reduce(f_plus,   ok_cisla, parametry); }
Symbol *krat  (List *parametry) { return vnitrni_reduce(f_krat,   ok_cisla, parametry); }
Symbol *minus (List *parametry) { return vnitrni_reduce(f_minus,  ok_cisla, parametry); }
Symbol *deleno(List *parametry) { return vnitrni_reduce(f_deleno, ok_cisla, parametry); }


Symbol *ok_cisla(Symbol *(*operace)(t_cislo, t_cislo), Symbol *a, Symbol *b)
{
	if (a == NULL || b == NULL) ERROR(PRAZDNA_HODNOTA);
	if (a->typ != CISLO || b->typ != CISLO) ERROR(OPERACE_NEMA_SMYSL);

	return operace(a->s.cislo, b->s.cislo);
}


/**
 * Funkce pro porovnavani
 * ----------------------
 */

static inline Symbol *is_not_null(Symbol *vysl)
{
	if (vysl != NULL && vysl->typ != NIL)
		return new_Ordinal(BOOL, BOOL_TRUE);
	else
		return new_Ordinal(BOOL, BOOL_FALSE);
}


static inline Symbol *f_eq(t_cislo a, t_cislo b)
{
	return (a == b) ? new_Ordinal(CISLO, a) : new_NIL(); 
}


static inline Symbol *f_gt(t_cislo a, t_cislo b)
{
	return (a > b) ? new_Ordinal(CISLO, b) : new_NIL();
}


Symbol *eq(List *parametry)
{
	return is_not_null(vnitrni_reduce(f_eq, ok_cisla, parametry));
}


Symbol *gt(List *parametry)
{
	return is_not_null(vnitrni_reduce(f_gt, ok_cisla, parametry));
}


Symbol *op_if(List *parametry)
{
	Symbol *s = resolve_Tank(parametry->symbol);

	if (s->typ != BOOL) ERROR(OPERACE_NEMA_SMYSL);

	if (s->s.boolean)
		return parametry->dalsi->symbol;
	else
		return parametry->dalsi->dalsi->symbol;
}


/**
 * Dalsi pomocne funkce
 * --------------------
 */

static Symbol *vnitrni_reduce(
		Symbol *(*operace)(void),
		Symbol *(*overeni)(Symbol *(*operace)(void), Symbol *, Symbol *),
		List *l
	)
{
	if (l == NULL) ERROR(VNITRNI_CHYBA);

	Symbol *s = l->symbol;
	l = l->dalsi;

	while (l != NULL && s != NULL) {	
		s = overeni(operace, resolve_Tank(s), resolve_Tank(l->symbol));
		l= l->dalsi;
	}

	return s;
}
