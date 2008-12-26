#include <stdlib.h>

#include "funkce.h"
#include "structs.h"
#include "error.h"


static Symbol *vnitrni_reduce(
		Symbol *(),
		Symbol *(*overeni)(Symbol *(), Symbol *, Symbol *),
		List *l
	);

static List *f_append(List *a, List *b);


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



// TODO misto pro optimalizace prevedenim na pole
Symbol *get_parametr(List *parametry, int kolikaty)
{
	if (kolikaty < 1) return NULL;

	while (kolikaty > 1) {
		parametry = parametry->dalsi;
		kolikaty--;
	}

	return parametry->symbol;
}


List *doplnit_parametry(List *parametry, List *kam)
{
	List *volani = new_List(NULL);
	List *vysledek = volani;
	List *l = kam;
	Symbol *s;

	while (l != NULL) {
		if (l->symbol->typ == LIST) {
			s = new_Symbol(LIST,
				doplnit_parametry(parametry, (List *)l->symbol->s.odkaz));
		} else if (l->symbol->typ == PARAMETR) {
			s = get_parametr(parametry, l->symbol->s.znak);
		} else {
			s = l->symbol;
		}

		volani->dalsi = new_List(s);
		volani = volani->dalsi;
		l = l->dalsi;
	}

	l = vysledek->dalsi;
	free(vysledek);
	return l; 
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


List *clone_List(List *l /*, int kolik */)
{
	if (l == NULL /*|| kolik == 0*/) return NULL;

	List *nl = new_List(l->symbol);

	while (l->dalsi != NULL /* && kolik > 0 */) {
		l = l->dalsi;
		nl->dalsi = new_List(l->symbol);
		nl = nl->dalsi;
		/* kolik--; */
	}

	return nl;
}


Symbol *call(List *l)
{
	if (l == NULL) return NULL;
	if (l->symbol == NULL) return new_Symbol(LIST, l);

	Funkce *f;

	if (l->symbol->typ == TANK) {
		f = ((Tank *)l->symbol->s.odkaz)->funkce;
		// potencionalni leak, protoze zmizi odkaz na l
		l = f_append( ((Tank *)l->dalsi->symbol->s.odkaz)->parametry, l);
	} else if (l->symbol->typ == FUNKCE) {
		f = (Funkce *)l->symbol->s.odkaz;
		l = l->dalsi;
	} else
		ERROR(VNITRNI_CHYBA);

	return new_Symbol_Tank(f, l);
}


Symbol *result(Funkce *f, List *parametry)
{
	if (f == NULL) return NULL;

	List *l = parametry;

	if (delka_listu(parametry) < f->pocet_parametru)
		return new_Symbol(TANK, new_Tank(f, /*clone_List(*/ parametry /*)*/));

	if (f->built_in == BUILT_IN)
		return f->telo.odkaz(parametry);


	// konvence je takova, ze prvni prvek listu vzdy odpovida bud:
	// - funkci/tanku = jde o volani funkce
	// - odkaz na funkci list/NULL (nikoliv NIL) = jde o list
	l = doplnit_parametry(parametry, f->telo.struktura);

	if (l->symbol->typ == TANK)
		l->symbol = resolve_Tank(l->symbol);

	return call(l);

	// nejsou vyreseny vnorene struktury:
	//
	// (a b c d (e f g) h i j) = pro e Tank/Funkci
}


Symbol *resolve_Tank(Symbol *s)
{
	if (s == NULL) return NULL;
	if (s->typ == LIST) s = call((List *)s->s.odkaz);
	if (s == NULL || s->typ != TANK) return s;


	Tank *t = (Tank *)s->s.odkaz;

	while (t != NULL
		&& delka_listu(t->parametry) >= t->funkce->pocet_parametru)
	{
		s = result(t->funkce, t->parametry);
		t = (s != NULL && s->typ == TANK) ? (Tank *)s->s.odkaz : NULL;
	}

	return s;
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

	return new_Symbol(LIST, l->dalsi);
}


Symbol *list(List *parametry)
{
	List *l = new_List(NULL);
	l->dalsi = parametry;
	return new_Symbol(LIST, l);
}


static List *f_append(List *a, List *b)
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

	l = ret->dalsi;
	free(ret);
	return l;
}


static inline Symbol *f_append_operace(List *a, List *b)
{
	return new_Symbol(LIST, f_append(a, b));
}


Symbol *ok_listy(Symbol *(*operace)(List *, List *), Symbol *a, Symbol *b)
{
	if (a == NULL || b == NULL) ERROR(PRAZDNA_HODNOTA);

	return operace( (a->typ == LIST) ? (List *)a->s.odkaz : new_List(a),
			(b->typ == LIST) ? (List *)b->s.odkaz : new_List(b));
}


Symbol *append(List *parametry)
{
	return vnitrni_reduce(f_append_operace, ok_listy, parametry);
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

static inline Symbol *f_eq(t_cislo a, t_cislo b)
{
	return (a == b) ? new_Ordinal(CISLO, a) : new_Symbol(NIL, NULL); 
}


static inline Symbol *f_gt(t_cislo a, t_cislo b)
{
	return (a > b) ? new_Ordinal(CISLO, b) : new_Symbol(NIL, NULL);
}


Symbol *eq(List *parametry)
{
	Symbol *vysl = vnitrni_reduce(f_eq, ok_cisla, parametry);

	if (vysl != NULL && vysl->typ != NIL)
		return new_Ordinal(BOOL, BOOL_TRUE);
	else
		return new_Ordinal(BOOL, BOOL_FALSE);
}


Symbol *gt(List *parametry)
{
	Symbol *vysl = vnitrni_reduce(f_gt, ok_cisla, parametry);

	if (vysl != NULL && vysl->typ != NIL)
		return new_Ordinal(BOOL, BOOL_TRUE);
	else
		return new_Ordinal(BOOL, BOOL_FALSE);
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
