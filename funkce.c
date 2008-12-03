#include <stdlib.h>

#include "funkce.h"
#include "structs.h"



Funkce **get_array_of_funtions()
{
	// - vytvoreni instanci vsech built-in funkci
	// - provazani techto instanci prasacky pres pointery na 
	//   opravdove funkce s pridanim priznaku built-in

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


Symbol *call(Funkce *f, List *parametry)
{
	List *l = parametry;

	int i = 0;
	while (l != NULL) {
		i++; l = l->dalsi;
	}

	// FIXME
	if (i < f->pocet_parametru) return new_Symbol(TANK, new_Tank(f, parametry));


	if (f->built_in == BUILT_IN) {
		return f->telo.odkaz(parametry);

	} else {
		l = f->telo.struktura;

	}




	return NULL;
}


Symbol *reduce(Funkce *f, List *l)
{
	if (l == NULL) return NULL;

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

List *head(List *l)
{
	if (l == NULL) return NULL;

	return (List *)l->symbol;
}


List *tail(List *l)
{
	if (l == NULL || l->dalsi == NULL)
		return NULL;

	return l->dalsi;
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
	if (l->dalsi == NULL) return 0; // FIXME

	if (!is_symbol_cislo(l->symbol) || !is_symbol_cislo(l->dalsi->symbol))
		return 0;

	int a = ((Hodnota *)l->symbol->odkaz)->h.cislo;
	int b = ((Hodnota *)l->dalsi->symbol->odkaz)->h.cislo;

	int vysledek = (*operace)(a, b);
	return new_Symbol(HODNOTA, new_Hodnota(CISLO, vysledek));
}
