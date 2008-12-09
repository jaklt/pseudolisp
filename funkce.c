#include <stdlib.h>

#include "funkce.h"
#include "structs.h"

static List *f_append(List *a, List *b);


Funkce **get_array_of_funtions()
{
	// - vytvoreni instanci vsech built-in funkci

	#define n 4
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
	}

	return array;
}



// eventuelni misto pro optimalizace prevedenim na pole
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
			s = get_parametr(parametry, l->symbol->s.cislo);
		//	s = new_Symbol(s->typ, ???);
		} else {
			s = l->symbol;
		}

		volani->dalsi = new_List(s);
		volani = volani->dalsi;

		l = l->dalsi;
	}

	l = vysledek->dalsi;
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


Symbol *result(List *l)
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
		return NULL;

	return new_Symbol_Tank(f, l);
}


Symbol *call(Funkce *f, List *parametry)
{
	if (f == NULL) return NULL;

	List *l = parametry;

	if (delka_listu(parametry) < f->pocet_parametru)
		return new_Symbol(TANK, new_Tank(f, clone_List(parametry)));

	if (f->built_in == BUILT_IN)
		return f->telo.odkaz(parametry);


	// konvence je takova, ze prvni prvek listu vzdy odpovida bud:
	// - funkci/tanku = jde o volani funkce
	// - odkaz na funkci list/NULL (nikoliv NIL) = jde o list
	l = doplnit_parametry(parametry, f->telo.struktura);

	if (l->symbol->typ == TANK)
		l->symbol = resolve_Tank(l->symbol);

	return result(l);

	// nejsou vyreseny vnorene struktury:
	//
	// (a b c d (e f g) h i j) = pro e Tank/Funkci
}


// bude hodit pozdej, ale rozhodne nepouzivat v call :-(
Symbol *resolve_Tank(Symbol *s)
{
	if (s == NULL || s->typ != TANK) return s;

	Tank *t = (Tank *)s->s.odkaz;

	while (s->typ == TANK 
		&& delka_listu(t->parametry) >= t->funkce->pocet_parametru) {

		s = call(t->funkce, t->parametry);
		t = (s->typ == TANK) ? (Tank *)s->s.odkaz : NULL;
	}

	return s;
}


Symbol *reduce(Funkce *f, List *l)
{
	if (l == NULL || f == NULL) return NULL;

	List *nl = new_List(l->symbol);
	nl->dalsi = new_List(NULL);
	l = l->dalsi;

	while (l != NULL) {	
		if (l->symbol->typ == TANK) call(NULL, NULL); // XXX
		nl->dalsi->symbol = l->symbol;
		nl->symbol = call(f, nl);

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
	return new_Symbol(LIST, parametry);
}


// TODO vic hezky
static List *f_append(List *a, List *b)
{
	List *result = new_List(NULL);
	List *l = result;

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

	return result->dalsi;
}

Symbol *append(List *parametry)
{
	return NULL;
}


/**
 * Aritmeticke operace s cisly
 * ---------------------------
 */

static int f_plus(int a, int b) { return a+b; }

Symbol *plus(List *parametry)
{
	return operace_s_cisly(f_plus, parametry);
}


static int f_krat(int a, int b) { return a*b; }

Symbol *krat(List *parametry)
{
	return operace_s_cisly(f_krat, parametry);
}


static int f_minus(int a, int b) { return a-b; }

Symbol *minus(List *parametry)
{
	return operace_s_cisly(f_minus, parametry);
}


static int f_deleno(int a, int b) { return a/b; }

Symbol *deleno(List *parametry)
{
	return operace_s_cisly(f_deleno, parametry);
}


static int is_symbol_cislo(Symbol *s)
{
	if (s == NULL) return 0;
	if (s->typ != CISLO) return 0;

	return 1;
}

// TODO napsat vic obecne pro jakykoliv typ a funkci
Symbol *operace_s_cisly(int (*operace)(int, int), List *l)
{
	if (l->dalsi == NULL) return NULL; // FIXME

	// TODO poresit tanky
	if (!is_symbol_cislo(l->symbol) || !is_symbol_cislo(l->dalsi->symbol))
		return 0;

	int a = l->symbol->s.cislo;
	int b = l->dalsi->symbol->s.cislo;

	int vysledek = (*operace)(a, b);
	return new_Ordinal(CISLO, vysledek);
}
