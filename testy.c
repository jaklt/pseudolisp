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
	funkcni_testy();
	testy_slozenych_funkci();

	return 0;
}


static int testy_slozenych_funkci()
{
	// 	plus minus krat deleno
	Funkce **f = get_array_of_funtions();

	int parametr = 1;
	List *telo = new_List(new_Symbol(FUNKCE, f[0]));
	telo->dalsi = new_List(new_Symbol(HODNOTA, new_Hodnota(CISLO, 3)));
	telo->dalsi->dalsi = new_List(new_Symbol(PARAMETR, &parametr));

	Funkce *a = new_Funkce(telo, 1);



	return 0;
}


static int funkcni_testy()
{
	Funkce **f = get_array_of_funtions();
	const int n = 4;
	Symbol *s[n];

	for (int i=0; i<n; i++) {
		s[i] = new_Symbol(HODNOTA, new_Hodnota(CISLO, i+3));
	}

	List *l = array_to_List(s, n);

//	Symbol *vysl = krat(l);
	Symbol *vysl = call(f[3], l);
	printf("vysledek: %d\n", ((Hodnota *)vysl->odkaz)->h.cislo);

	return 0;
}


static int listove_testy()
{
	const int n = 10;
	Symbol *s[n];
	Hodnota *h;
	List *l;

	for (int i=0; i<n; i++) {
		h = new_Hodnota(CISLO, i*100);
		s[i] = new_Symbol(HODNOTA, h);
	}

	l = array_to_List(s, n);
	vypis_list(l);

	return 0;
}


static int vypis_list(List *l)
{
	Hodnota *h;
	while (l != NULL) {
		h = (Hodnota *) (l->symbol->odkaz);
		printf("%d %3d\n", h->typ, h->h.cislo);

		l = l->dalsi;
	}

	return 0;
}

