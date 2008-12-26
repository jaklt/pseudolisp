#include <stdio.h>
// #include <stdlib.h>

#include "funkce.h"
#include "structs.h"


static int vypis_list(List *l);
static int listove_testy();
static int funkcni_testy();
static int testy_slozenych_funkci();
static int zkouska_erroru();
static int logicke_vyrazy();
static int testovani_if();
#define NEXT printf("\n-- dalsi test:\n")
#define SPRAVNE(a) printf("\t** SPRAVNE: %s **\n", a)

int test()
{
	printf("-- Zacatek testovani:\n");
//	listove_testy(); NEXT;
//	funkcni_testy(); NEXT;
//	testy_slozenych_funkci(); NEXT;
//	zkouska_erroru(); NEXT;
//	logicke_vyrazy(); NEXT;
	testovani_if();

	return 0;
}


static int testovani_if()
{
	Funkce **f = get_array_of_funtions();

	List *porovnani = new_List(new_Symbol(FUNKCE, f[6]));
	porovnani->dalsi = new_List(new_Ordinal(PARAMETR, 1));
	porovnani->dalsi->dalsi = new_List(new_Ordinal(PARAMETR, 2));

	List *telo = new_List(new_Symbol(FUNKCE, f[4]));
	telo->dalsi = new_List(new_Symbol(LIST, porovnani));
	telo->dalsi->dalsi = new_List(new_Ordinal(PARAMETR, 1));
	telo->dalsi->dalsi->dalsi = new_List(new_Ordinal(PARAMETR, 2));

	Symbol *fce = new_Symbol_Funkce(telo, 2);

	List *volani = new_List(fce);
	volani->dalsi = new_List(new_Ordinal(CISLO, 2));
	volani->dalsi->dalsi = new_List(new_Ordinal(CISLO, 3));

	Symbol *vysl = resolve_Tank(call(volani));
	printf("typ vysledku: %d\n", vysl->typ);
	printf("vysledek: %d\n", (int)vysl->s.cislo);

	SPRAVNE("typ 3");
	return 0;
}


static int logicke_vyrazy()
{
	Funkce **f = get_array_of_funtions();

	List *telo = new_List(new_Symbol(FUNKCE, f[6]));
	telo->dalsi = new_List(new_Ordinal(CISLO, 12));
	telo->dalsi->dalsi = new_List(new_Ordinal(CISLO, 11));

	Symbol *vysl = resolve_Tank(call(telo));

	printf("typ vysledku: %d\n", vysl->typ);
	printf("vysledek: %s\n", vysl->s.boolean ? "TRUE" : "FALSE");

	SPRAVNE("typ 4, vysledek TRUE");

	printf("\n");

	List *telo2 = new_List(new_Symbol(FUNKCE, f[5]));
	telo2->dalsi = new_List(new_Ordinal(CISLO, 11));
	telo2->dalsi->dalsi = new_List(new_Ordinal(CISLO, 11));

	vysl = resolve_Tank(call(telo2));

	printf("typ vysledku: %d\n", vysl->typ);
	printf("vysledek: %s\n", vysl->s.boolean ? "TRUE" : "FALSE");
	SPRAVNE("typ 4, vysledek TRUE");

	printf("\n");

	List *okif = new_List(new_Symbol(FUNKCE, f[4]));
	okif->dalsi = new_List(new_Ordinal(BOOL, BOOL_FALSE));
	okif->dalsi->dalsi = new_List(new_Ordinal(CISLO, 1));
	okif->dalsi->dalsi->dalsi = new_List(new_Ordinal(CISLO, 2));

	vysl = resolve_Tank(call(okif));
	printf("typ vysledku: %d\n", vysl->typ);
	printf("vysledek: %d\n", (int)vysl->s.cislo);
	SPRAVNE("typ 3, vysledek 2");

	return 0;
}


static int zkouska_erroru()
{
	Funkce **f = get_array_of_funtions();

	List *telo = new_List(new_Symbol(FUNKCE, f[0]));
	telo->dalsi = new_List(new_Ordinal(PARAMETR, 1));
	telo->dalsi->dalsi = new_List(new_Ordinal(PARAMETR, 2));

	Funkce *a = new_Funkce(telo, 2);

	List *do_tanku = new_List(new_Symbol(FUNKCE, f[0]));
	do_tanku->dalsi = new_List(new_Ordinal(CISLO, 3));
	do_tanku->dalsi->dalsi = new_List(new_Ordinal(CISLO, 5));


	List *volani = new_List(new_Ordinal(CISLO, 2));
	volani->dalsi = new_List(new_Symbol(LIST, do_tanku));

	Symbol *vysledek = resolve_Tank(result(a, volani));

	printf("vysledek-error: %s\n", (vysledek == NULL) ? "JO" : "NE");
	printf("vysledek-error-kolko: %d\n", (int) vysledek->s.cislo);
	printf("vysledku-error2: %s\n", (resolve_Tank(call(do_tanku)) == NULL) ? "JO" : "NE");

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

	Symbol *vysledek = resolve_Tank(result(a, new_List(new_Ordinal(CISLO, 8))));
	printf("typ vysledku: %d ", vysledek->typ);
	printf("parametr: %d\n", (int)a->telo.struktura->dalsi->dalsi->symbol->s.znak);
	printf("vysledek: %d\n", (int) vysledek->s.cislo);

	SPRAVNE("typ 3, parametr 1, vysledek 11");
	return 0;
}


static int funkcni_testy()
{
	Funkce **f = get_array_of_funtions();
	const int n = 5;
	Symbol *s[n];

	for (int i=0; i<n; i++) {
		s[i] = new_Ordinal(CISLO, i+3);
	}

	List *l = array_to_List(s, n);
	vypis_list(l);

//	Symbol *vysl = krat(l);
	Symbol *vysl = result(f[0], l);
	printf("vysledek: %d\n", (int) vysl->s.cislo);

	SPRAVNE("25");
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

/*	List *nl = new_List(new_Symbol(LIST, l));
	nl->dalsi = new_List(new_Symbol(LIST, l));
	Symbol *list = append(nl);
	if (list != NULL && list->typ == LIST) vypis_list((List *)list->s.odkaz);
*/
	return 0;
}


static int vypis_list(List *l)
{
	while (l != NULL) {
		printf("%d %3d\n", l->symbol->typ, (int) l->symbol->s.cislo);

		l = l->dalsi;
	}

	return 0;
}

