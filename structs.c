#include <stdlib.h>
#include "structs.h"



Funkce *new_Funkce(List *telo_funkce, int pocet_parametru)
{
	Funkce *f = (Funkce *) malloc(sizeof(Funkce));

	f->telo.struktura = telo_funkce;
	f->pocet_parametru = pocet_parametru;
	f->built_in = NOT_BUILT_IN;

	return f;
}


List *new_List(Symbol *symbol)
{
	List *l = (List *) malloc(sizeof(List));

	l->symbol = symbol;
	l->dalsi = NULL;

	return l;
}


List *array_to_List(Symbol **seznam_symbolu, int pocet_symbolu)
{
	if (pocet_symbolu <= 0) return NULL;

	List *l = (List *) malloc(pocet_symbolu * sizeof(List));
	List *ret = l;

	for (int i=0; i<pocet_symbolu; i++) {
		l->symbol = (Symbol *) seznam_symbolu[i];
		l->dalsi = NULL;
		
		if (i != 0) (l-1)->dalsi = l;
		l++;
	}

	l->dalsi = NULL;

	return ret;
}


Hodnota *new_Hodnota(E_HODNOTA typ, int co)
{
	Hodnota *h = (Hodnota *) malloc(sizeof(Hodnota));

	h->typ = typ;

	switch (typ) {
		case CISLO: h->h.cislo = co; break;
		case BOOL: h->h.boolean = co; break;
		case ZNAK:  h->h.znak = (char) co; break;
	}

	return h;

}


Symbol *new_Symbol(E_TYP typ, void *symbol)
{
	Symbol *s = (Symbol *) malloc(sizeof(Symbol));

	s->typ = typ;
	s->odkaz = (void *)symbol;

	return s;
}


Tank *new_Tank(Funkce *fce, List *parametry)
{
	Tank *t = (Tank *) malloc(sizeof(Tank));

	t->funkce = fce;
	t->parametry = parametry;

	return t;
}
