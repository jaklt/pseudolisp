#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"

static int odsazeni = 0;


static int odsadit()
{
	for (int i=0; i<odsazeni; i++)
		printf("  ");

	return 0;
}


char *new_temp_name()
{
	static unsigned int i = 0;
	i++;

	char *c = malloc(sizeof(char)*7);
	c[0] = 't';
	c[1] = 'm';
	c[2] = 'p';

	c[3] = ((i / 100) % 10) + 48;
	c[4] = ((i / 10)  % 10) + 48;
	c[5] = ( i        % 10) + 48;

	c[6] = '\0';

	return c;
}


int vypis_Typ(E_TYP t)
{
	switch (t) {
		case FUNKCE: printf("Type: Function\n"); break;
		case   LIST: printf("Type: List\n"); break;
		case   TANK: printf("Type: Thunk\n"); break;
		case    NIL: printf("Type: NIL\n"); break;
		case   BOOL: printf("Type: Boolean\n"); break;
		case   ZNAK: printf("Type: Char\n"); break;
		case  CISLO: printf("Type: Number\n"); break;
		case PARAMETR: printf("Type: Parametr\n"); break;
		default:     printf("Wrong type"); return 1; 
	}

	return 0;
}


int vypis_Symbol(Symbol *s)
{
	if (s == NULL) {
		printf("Empty Symbol\n");
		return 1;
	}

	switch (s->typ) {
		case FUNKCE: vypis_Funkce((Funkce *) s->s.odkaz); break;
		case  LIST: vypis_List((List *) s->s.odkaz); break;
		case  TANK: vypis_Tank((Tank *) s->s.odkaz); break;
		case   NIL: printf("NIL\n"); break;
		case  BOOL: printf("Bool: %s\n", (s->s.boolean == BOOL_TRUE) ? "TRUE" : "FALSE"); break;
		case  ZNAK: printf("Char: %c\n", s->s.znak); break;
		case CISLO: printf("Number: %f\n", (double) s->s.cislo); break;
		case PARAMETR: printf("%d. parametr\n", (int) s->s.znak); break;
		default: printf("Wrong value!\n"); break;
	}

	return 0;
}


int vypis_List(List *l)
{
	printf("List:\n");
	if (l == NULL) return 1;
	odsazeni++;

	while (l != NULL) {
		odsadit();
		vypis_Symbol(l->symbol);

		l = l->dalsi;
	}

	odsazeni--;
	return 0;
}


int vypis_Funkce(Funkce *f)
{
	printf("Function:\n");
	if (f == NULL) return 1;
	odsazeni++;

	odsadit(); printf("Number of parametrs: %d\n", f->pocet_parametru);

	if (f->built_in) {
		odsadit(); printf("Build in\n");
	} else {
		odsadit(); printf("Function body - "); vypis_List(f->telo.struktura);
	}

	odsazeni--;
	return 0;
}


int vypis_Tank(Tank *t)
{
	printf("Thunk:\n");
	if (t == NULL) return 1;
	odsazeni++;

	odsadit(); vypis_Funkce(t->funkce);
	odsadit(); printf("Parametrs - "); vypis_List(t->parametry);

	odsazeni--;
	return 0;
}
