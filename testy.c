#include <stdio.h>
// #include <stdlib.h>

#include "funkce.h"
#include "structs.h"


static int vypis_list(List *l);
static int listove_testy();
static int funkcni_testy();
static int testy_slozenych_funkci();

int test()
{
//	listove_testy();
//	funkcni_testy();
	testy_slozenych_funkci();

	return 0;
}


static int testy_slozenych_funkci()
{
	// 	plus minus krat deleno
	Funkce **f = get_array_of_funtions();

	List *telo = new_List(new_Symbol(FUNKCE, f[0]));
	telo->dalsi = new_List(new_Ordinal(CISLO, 3));
	telo->dalsi->dalsi = new_List(new_Ordinal(PARAMETR, 1));

	Funkce *a = new_Funkce(telo, 1);

	Symbol *vysledek = resolve_Tank(call(a, new_List(new_Ordinal(CISLO, 8))));
	printf("typ vysledku: %d ", vysledek->typ);
	printf("parametr: %d\n", a->telo.struktura->dalsi->dalsi->symbol->s.cislo);
	printf("vysledek: %d\n", (int)vysledek->s.cislo);

	return 0;
}


static int funkcni_testy()
{
	Funkce **f = get_array_of_funtions();
	const int n = 4;
	Symbol *s[n];

	for (int i=0; i<n; i++) {
		s[i] = new_Ordinal(CISLO, i+3);
	}

	List *l = array_to_List(s, n);

//	Symbol *vysl = krat(l);
	Symbol *vysl = call(f[0], l);
	printf("vysledek: %d\n", vysl->s.cislo);

	return 0;
}


static int listove_testy()
{
	const int n = 10;
	Symbol *s[n];
	List *l;

	for (int i=0; i<n; i++) {
		s[i] = new_Ordinal(CISLO, i*100);
	}

	l = array_to_List(s, n);
	vypis_list(l);

	return 0;
}


static int vypis_list(List *l)
{
	while (l != NULL) {
		printf("%d %3d\n", l->symbol->typ, l->symbol->s.cislo);

		l = l->dalsi;
	}

	return 0;
}

