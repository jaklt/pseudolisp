#include <stdio.h>

#include "funkce.h"
#include "structs.h"
#include "helpers.h"
#include "execute.h"
#include "parser.h"
#include "hashovani.h"


static Function **get_array_of_funtions();
static int funkcni_testy();
static int testy_slozenych_funkci();
static int zkouska_erroru();
static int logicke_vyrazy();
static int testovani_if();
static int listove_testy();
static int nekonecne_testy();
#define NEXT printf("\n-- next test:\n")
#define SPRAVNE(a) printf("\t** SPRAVNE: "); printf(a); printf(" **\n");
#define SPRAVNE_NUMBER(a,b) printf("\t** SPRAVNE: "); printf(a,b); printf(" **\n");


int test()
{
	printf("-- Zacatek testovani:\n");
	funkcni_testy(); NEXT;
	testy_slozenych_funkci(); NEXT;
	zkouska_erroru(); NEXT;
	logicke_vyrazy(); NEXT;
	testovani_if(); NEXT;
	listove_testy(); NEXT;
	nekonecne_testy(); NEXT;

	return 0;
}


static int nekonecne_testy()
{
	Function *f = new_Function(NULL, 1);
	Function **funkce = get_array_of_funtions();

	// [def count [l] [tail [count NIL]]] [count NIL]
	List *rec = new_List(new_Symbol(FUNCTION, f));
	rec->next = new_List(new_NIL());

	List *body = new_List(new_Symbol(FUNCTION, funkce[8]));
	body->next = new_List(new_Symbol(LIST, rec));

	f->body.structure = body;

	List *calling = new_List(new_Symbol(FUNCTION, f));
	calling->next = new_List(new_NIL());

	print_Symbol(resolve_Thunk(call(calling)));

	return 0; 
}


static int listove_testy()
{
	const int n = 10;
	List *l = new_List(NULL);
	List *nl = l;

	for (int i=1; i<n; i++) {
		nl->next = new_List(new_Ordinal(NUMBER, i*100));
		nl = nl->next;
	}

	nl = new_List(new_Symbol(LIST, l));
	nl->next = new_List(new_Symbol(LIST, l));
	Symbol *list = append(nl);

//	print_Symbol(list);

	nl->symbol = new_NIL();
	nl->next->next = new_List(new_Ordinal(CHAR, 'A'));
	nl->next->next->next = new_List(new_Symbol(LIST, new_List(NULL)));
	list = append(nl);
	print_Symbol(list);

	return 0;
}


static int testovani_if()
{
	Function **f = get_array_of_funtions();

	List *porovnani = new_List(new_Symbol(FUNCTION, f[6]));
	porovnani->next = new_List(new_Ordinal(PARAMETR, 1));
	porovnani->next->next = new_List(new_Ordinal(PARAMETR, 2));

	List *body = new_List(new_Symbol(FUNCTION, f[4]));
	body->next = new_List(new_Symbol(LIST, porovnani));
	body->next->next = new_List(new_Ordinal(PARAMETR, 1));
	body->next->next->next = new_List(new_Ordinal(PARAMETR, 2));

	Symbol *fce = new_Symbol(FUNCTION, new_Function(body, 2));

	List *volani = new_List(fce);
	volani->next = new_List(new_Ordinal(NUMBER, 2));
	volani->next->next = new_List(new_Ordinal(NUMBER, 3));

	Symbol *vysl = resolve_Thunk(call(volani));
	print_Symbol(vysl);
	SPRAVNE("Number = 3");

	return 0;
}


static int logicke_vyrazy()
{
	Function **f = get_array_of_funtions();

	List *body = new_List(new_Symbol(FUNCTION, f[6]));
	body->next = new_List(new_Ordinal(NUMBER, 12));
	body->next->next = new_List(new_Ordinal(NUMBER, 11));

	Symbol *vysl = resolve_Thunk(call(body));
	print_Symbol(vysl);
	SPRAVNE("TRUE");

	printf("\n");

	List *body2 = new_List(new_Symbol(FUNCTION, f[5]));
	body2->next = new_List(new_Ordinal(NUMBER, 11));
	body2->next->next = new_List(new_Ordinal(NUMBER, 11));

	vysl = resolve_Thunk(call(body2));
	print_Symbol(vysl);
	SPRAVNE("TRUE");

	printf("\n");

	List *okif = new_List(new_Symbol(FUNCTION, f[4]));
	okif->next = new_List(new_Ordinal(BOOL, BOOL_FALSE));
	okif->next->next = new_List(new_Ordinal(NUMBER, 1));
	okif->next->next->next = new_List(new_Ordinal(NUMBER, 2));

	vysl = resolve_Thunk(call(okif));
	print_Symbol(vysl);
	SPRAVNE("Number = 2");

	return 0;
}


static int zkouska_erroru()
{
	Function **f = get_array_of_funtions();

	List *body = new_List(new_Symbol(FUNCTION, f[0]));
	body->next = new_List(new_Ordinal(PARAMETR, 1));
	body->next->next = new_List(new_Ordinal(PARAMETR, 2));

	Function *a = new_Function(body, 2);

	List *do_tanku = new_List(new_Symbol(FUNCTION, f[0]));
	do_tanku->next = new_List(new_Ordinal(NUMBER, 3));
//	do_tanku->next->next = new_List(new_Ordinal(NUMBER, 5));
	do_tanku->next->next = new_List(new_NIL());


	List *volani = new_List(new_Ordinal(NUMBER, 2));
	volani->next = new_List(new_Symbol(LIST, do_tanku));

	Symbol *vysledek = resolve_Thunk(result(a, volani));

	printf("vysledek-error: %s\n", (vysledek == NULL) ? "JO" : "NE");
	print_Symbol(vysledek);
	printf("vysledek-error2: %s\n", (resolve_Thunk(call(do_tanku)) == NULL) ? "JO" : "NE");

	return 0;
}


static int testy_slozenych_funkci()
{
	// 	plus minus krat deleno
	Function **f = get_array_of_funtions();

	List *body = new_List(new_Symbol(FUNCTION, f[0]));
	body->next = new_List(new_Ordinal(NUMBER, 3));
	body->next->next = new_List(new_Ordinal(PARAMETR, 1));

	//*
	Function *a = new_Function(body, 1);

	Symbol *vysledek = resolve_Thunk(result(a, new_List(new_Ordinal(NUMBER, 8))));
	print_Symbol(vysledek);
	printf("parametr: %d\n", (int)a->body.structure->next->next->symbol->s.character);

	SPRAVNE("Number = 11, parametr 1");
	printf("\n"); // */

	body->next->next = NULL;
	List *calling = new_List(new_Symbol(LIST, body));
	calling->next = new_List(new_Ordinal(NUMBER, 3));

	print_Symbol(resolve_Thunk(call(calling)));

	SPRAVNE("Number = 6");
	return 0;
}


static int funkcni_testy()
{
	Function **f = get_array_of_funtions();
	const int n = 5;
	List *l = new_List(NULL);
	List *nl = l;

	for (int i=0; i<n; i++) {
		nl->next = new_List(new_Ordinal(NUMBER, i+3));
		nl = nl->next;
	}

	print_List(l->next);

//	Symbol *vysl = krat(l);
	Symbol *vysl = result(f[0], l->next);
	print_Symbol(vysl);

	SPRAVNE("Number = 25");
	return 0;
}


static Function **get_array_of_funtions()
{
	// - vytvoreni instanci vsech built-in funkci

	#define n 10
	static Function *array[n];
	static int hotovson = 1;
	
	if (hotovson) {
		hotovson = 0;

		array[0] = new_Function(NULL, 2);
		array[0]->built_in = BOOL_TRUE;
		array[0]->body.link = plus;

		array[1] = new_Function(NULL, 2);
		array[1]->built_in = BOOL_TRUE;
		array[1]->body.link = minus;

		array[2] = new_Function(NULL, 2);
		array[2]->built_in = BOOL_TRUE;
		array[2]->body.link = krat;

		array[3] = new_Function(NULL, 2);
		array[3]->built_in = BOOL_TRUE;
		array[3]->body.link = deleno;

		array[4] = new_Function(NULL, 3);
		array[4]->built_in = BOOL_TRUE;
		array[4]->body.link = op_if;

		array[5] = new_Function(NULL, 2);
		array[5]->built_in = BOOL_TRUE;
		array[5]->body.link = eq;

		array[6] = new_Function(NULL, 2);
		array[6]->built_in = BOOL_TRUE;
		array[6]->body.link = gt;

		array[7] = new_Function(NULL, 2);
		array[7]->built_in = BOOL_TRUE;
		array[7]->body.link = append;

		array[8] = new_Function(NULL, 1);
		array[8]->built_in = BOOL_TRUE;
		array[8]->body.link = tail;

		array[9] = new_Function(NULL, 1);
		array[9]->built_in = BOOL_TRUE;
		array[9]->body.link = head;
	}

	return array;
}
