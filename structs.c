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


Symbol *new_Ordinal(E_TYP typ, int co)
{
	Symbol *s = (Symbol *) malloc(sizeof(Symbol));

	s->typ = typ;

	switch (typ) {
		case PARAMETR:
		case CISLO: s->s.cislo   =        co; break;
		case  BOOL: s->s.boolean =        co; break;
		case  ZNAK: s->s.znak    = (char) co; break;
		default: break;
	}

	return s;
}


Symbol *new_Symbol(E_TYP typ, void *symbol)
{
	Symbol *s = (Symbol *) malloc(sizeof(Symbol));

	s->typ = typ;
	s->s.odkaz = (void *)symbol;

	return s;
}


Tank *new_Tank(Funkce *fce, List *parametry)
{
	Tank *t = (Tank *) malloc(sizeof(Tank));

	t->funkce = fce;
	t->parametry = parametry;

	return t;
}
