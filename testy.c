#include <stdio.h>
#include <string.h>
// #include <stdlib.h>

#include "funkce.h"
#include "structs.h"
#include "helpers.h"
#include "execute.h"


static int listove_testy();
static int funkcni_testy();
static int testy_slozenych_funkci();
static int zkouska_erroru();
static int logicke_vyrazy();
static int testovani_if();
static int listove_testy();
#define NEXT printf("\n-- dalsi test:\n")
#define SPRAVNE(a) printf("\t** SPRAVNE: "); printf(a); printf(" **\n");
#define SPRAVNE_CISLO(a,b) printf("\t** SPRAVNE: "); printf(a,b); printf(" **\n");

int test()
{
	printf("-- Zacatek testovani:\n");
//	funkcni_testy(); NEXT;
//	testy_slozenych_funkci(); NEXT;
//	zkouska_erroru(); NEXT;
//	logicke_vyrazy(); NEXT;
//	testovani_if(); NEXT;
	listove_testy(); NEXT;

	return 0;
}


static int listove_testy()
{
	const int n = 10;
	Symbol *s[n];
	List *l;

	s[0] = NULL;
	for (int i=1; i<n; i++) {
		s[i] = new_Ordinal(CISLO, i*100);
	}

	l = array_to_List(s, n);
	vypis_List(l);

	List *nl = new_List(new_Symbol(LIST, l));
	nl->dalsi = new_List(new_Symbol(LIST, l));
	Symbol *list = append(nl);

	vypis_Symbol(list);

	nl->symbol = new_NIL();
	nl->dalsi->dalsi = new_List(new_Ordinal(ZNAK, 'A'));
	nl->dalsi->dalsi->dalsi = new_List(new_Symbol_List(NULL));
	list = append(nl);
	vypis_Symbol(list);

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
	vypis_Symbol(vysl);
	SPRAVNE("Number = 3");

	return 0;
}


static int logicke_vyrazy()
{
	Funkce **f = get_array_of_funtions();

	List *telo = new_List(new_Symbol(FUNKCE, f[6]));
	telo->dalsi = new_List(new_Ordinal(CISLO, 12));
	telo->dalsi->dalsi = new_List(new_Ordinal(CISLO, 11));

	Symbol *vysl = resolve_Tank(call(telo));
	vypis_Symbol(vysl);
	SPRAVNE("TRUE");

	printf("\n");

	List *telo2 = new_List(new_Symbol(FUNKCE, f[5]));
	telo2->dalsi = new_List(new_Ordinal(CISLO, 11));
	telo2->dalsi->dalsi = new_List(new_Ordinal(CISLO, 11));

	vysl = resolve_Tank(call(telo2));
	vypis_Symbol(vysl);
	SPRAVNE("TRUE");

	printf("\n");

	List *okif = new_List(new_Symbol(FUNKCE, f[4]));
	okif->dalsi = new_List(new_Ordinal(BOOL, BOOL_FALSE));
	okif->dalsi->dalsi = new_List(new_Ordinal(CISLO, 1));
	okif->dalsi->dalsi->dalsi = new_List(new_Ordinal(CISLO, 2));

	vysl = resolve_Tank(call(okif));
	vypis_Symbol(vysl);
	SPRAVNE("Number = 2");

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
//	do_tanku->dalsi->dalsi = new_List(new_Ordinal(CISLO, 5));
	do_tanku->dalsi->dalsi = new_List(new_NIL());


	List *volani = new_List(new_Ordinal(CISLO, 2));
	volani->dalsi = new_List(new_Symbol(LIST, do_tanku));

	Symbol *vysledek = resolve_Tank(result(a, volani));

	printf("vysledek-error: %s\n", (vysledek == NULL) ? "JO" : "NE");
	vypis_Symbol(vysledek);
	printf("vysledek-error2: %s\n", (resolve_Tank(call(do_tanku)) == NULL) ? "JO" : "NE");

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
	vypis_Symbol(vysledek);
	printf("parametr: %d\n", (int)a->telo.struktura->dalsi->dalsi->symbol->s.znak);

	SPRAVNE("Number = 11, parametr 1");
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
	vypis_List(l);

//	Symbol *vysl = krat(l);
	Symbol *vysl = result(f[0], l);
	vypis_Symbol(vysl);

	SPRAVNE("Number = 25");
	return 0;
}


