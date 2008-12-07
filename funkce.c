#include <stdlib.h>

#include "funkce.h"
#include "structs.h"



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



static int is_symbol_tank(Symbol *s)
{
	if (s == NULL) return 0;

	return s->typ == TANK;
}


static int is_symbol_cislo(Symbol *s)
{
	if (s == NULL) return 0;
	if (s->typ != HODNOTA) return 0;
	if ( ((Hodnota *)s->odkaz)->typ != CISLO) return 0;

	return 1;
}


// eventuelni misto pro optimalizace prevedenim na pole
Symbol *get_parametr(int kolikaty, List *parametry)
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
			s = new_Symbol(LIST, doplnit_parametry(parametry, (List *)l->symbol->odkaz));
		} else if (l->symbol->typ == PARAMETR) {
			s = get_parametr(*((int *)l->symbol->odkaz), parametry);
		//	s = new_Symbol(s->typ, s);
		} else {
			s = new_Symbol(l->symbol->typ, l->symbol->odkaz);
		}

		volani->dalsi = new_List(s);
		volani = volani->dalsi;

		l = l->dalsi;
	}

	// uvolnit(LIST, volani);
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


Symbol *result(List *telo)
{
	if (telo == NULL) return NULL;
	Funkce *f = NULL;
	List *parametry = NULL;

	if (telo->symbol->typ == FUNKCE) {
		f = (Funkce *)telo->symbol->odkaz;
		parametry = telo->dalsi;

	} else if (telo->symbol->typ == TANK){
		Tank *t = (Tank *)telo->symbol->odkaz;
		Symbol *s = new_Symbol(TANK, t);
		int pocet_parametru;

		while (s->typ == TANK) {
			pocet_parametru = delka_listu(t->parametry);

			if (pocet_parametru >= t->funkce->pocet_parametru)
				s = call(t->funkce, t->parametry);
			else
				/* ... */;
		}

	}

	// A jak resit kdyz mam [Tank a1 a2 a3 a4] a Tank spolu s a1 vrati tank??
	// - zavolat novej tank s tolika parametrama kolik bude potrebovat?
	// - vratit proste vysledek [Tank a1]?

	return NULL;
}


Symbol *call(Funkce *f, List *parametry)
{
	List *l = parametry;

	if (delka_listu(parametry) < f->pocet_parametru)
		return new_Symbol(TANK, new_Tank(f, clone_List(parametry)));


	if (f->built_in == BUILT_IN) {
		return f->telo.odkaz(parametry);

	} else {
		// konvence je takova, ze prvni prvek listu vzdy odpovida bud:
		// - funkci/tanku = jde o volani funkce
		// - odkaz na funkci list/NULL (nikoliv NIL) = jde o list
		l = doplnit_parametry(parametry, f->telo.struktura);
	}


	return new_Symbol(NIL, NULL);
}


// bude hodit pozdej, ale rozhodne nepouzivat v call :-(
Symbol *resolve_Tank(Tank *t)
{
	Symbol *s = new_Symbol(TANK, t);

	while (s->typ == TANK 
		&& delka_listu(t->parametry) >= t->funkce->pocet_parametru) {

		s = call(t->funkce, t->parametry);
		t = (s->typ == TANK) ? (Tank *)s->odkaz : NULL;
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
		if (is_symbol_tank(l->symbol)) call(NULL, NULL); // XXX
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


Symbol *operace_s_cisly(int (*operace)(int, int), List *l)
{
	if (l->dalsi == NULL) return NULL; // FIXME

	// TODO poresit tanky
	if (!is_symbol_cislo(l->symbol) || !is_symbol_cislo(l->dalsi->symbol))
		return 0;

	int a = ((Hodnota *)l->symbol->odkaz)->h.cislo;
	int b = ((Hodnota *)l->dalsi->symbol->odkaz)->h.cislo;

	int vysledek = (*operace)(a, b);
	return new_Symbol(HODNOTA, new_Hodnota(CISLO, vysledek));
}
