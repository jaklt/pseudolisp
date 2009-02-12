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


int print_Typ(E_TYP t)
{
	switch (t) {
		case FUNCTION: printf("Type: Function\n"); break;
		case   LIST: printf("Type: List\n"); break;
		case  THUNK: printf("Type: Thunk\n"); break;
		case    NIL: printf("Type: NIL\n"); break;
		case   BOOL: printf("Type: Boolean\n"); break;
		case   CHAR: printf("Type: Char\n"); break;
		case NUMBER: printf("Type: Number\n"); break;
		case PARAMETR: printf("Type: Parametr\n"); break;
		default:     printf("Wrong type"); return 1; 
	}

	return 0;
}


int print_Symbol(Symbol *s)
{
	if (s == NULL) {
		printf("Empty Symbol\n");
		return 1;
	}

	switch (s->typ) {
		case FUNCTION: print_Function((Function *) s->s.link); break;
		case  LIST: print_List((List *) s->s.link); break;
		case THUNK: print_Tank((Tank *) s->s.link); break;
		case   NIL: printf("NIL\n"); break;
		case  BOOL: printf("Bool: %s\n", (s->s.boolean == BOOL_TRUE) ? "TRUE" : "FALSE"); break;
		case  CHAR: printf("Char: %c\n", s->s.character); break;
		case NUMBER: printf("Number: %f\n", (double) s->s.number); break;
		case PARAMETR: printf("%d. parametr\n", (int) s->s.character); break;
		default: printf("Wrong value!\n"); break;
	}

	return 0;
}


int print_List(List *l)
{
	printf("List:\n");
	if (l == NULL) return 1;
	odsazeni++;

	while (l != NULL) {
		odsadit();
		print_Symbol(l->symbol);

		l = l->next;
	}

	odsazeni--;
	return 0;
}


int print_Function(Function *f)
{
	printf("Function:\n");
	if (f == NULL) return 1;
	odsazeni++;

	odsadit(); printf("Number of parametrs: %d\n", f->number_of_params);

	if (f->built_in) {
		odsadit(); printf("Build in\n");
	} else {
		odsadit(); printf("Function body - "); print_List(f->body.structure);
	}

	odsazeni--;
	return 0;
}


int print_Tank(Tank *t)
{
	printf("Thunk:\n");
	if (t == NULL) return 1;
	odsazeni++;

	odsadit(); print_Function(t->function);
	odsadit(); printf("Parametrs - "); print_List(t->params);

	odsazeni--;
	return 0;
}
