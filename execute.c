#include <stdlib.h>

#include "funkce.h"
#include "structs.h"
#include "error.h"
#include "execute.h"


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


List *clone_List(List *l)
{
	if (l == NULL) return NULL;

	List *nl = new_List(l->symbol);
	List *nll = nl;

	while (l->dalsi != NULL) {
		l = l->dalsi;
		nll->dalsi = new_List(l->symbol);
		nll = nll->dalsi;
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
		return new_Symbol(TANK, new_Tank(f, parametry));

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
