#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"
#include "execute.h"
#include "error.h"

static int odsazeni = 0;
static int resolve  = 1;


static int odsadit()
{
	for (int i=0; i<odsazeni; i++)
		printf("  ");

	return 0;
}


int print_Symbol(t_point s)
{
	if (resolve) s = resolve_Thunk(s);

	switch (get_type(s)) {
		case FUNCTION:
			if (s == BOOL_TRUE)
				printf("True\n");
			else
#ifndef DEBUG
				printf("Function: built in %d, %d%s parameters\n",
						(get_Func(s))->built_in, (get_Func(s))->params_count,
						(get_Func(s))->more_params ? "+" : "");
#else
				print_Function(get_Func(s));
#endif
			break;
		case THUNK:
			if (is_NIL(get_Thunk(s)->function))
				printf("%li. Parameter\n", get_Num((t_point) get_Thunk(s)->params));
			else
#ifndef DEBUG
				printf("Thunk\n");
#else
				print_Thunk(get_Thunk(s));
#endif
			break;
		case CONS:
			if (s == NIL)
				printf("NIL/False\n");
			else
				// TODO moznost naznacit, ze jde o List
#ifndef DEBUG
	  			printf("Cons\n");
#else
				print_Cons(get_Cons(s));
#endif
			break;
		case NUMBER: printf("Number: %li\n", get_Num(s)); break;
		default:     printf("Wrong type (%lx)\n", (t_number) s); return 1;
	}

	return 0;
}


int print_List(Cons *l)
{
	if (l == NULL) { printf("Empty List\n"); return 1; }
	odsazeni++;
	printf("List:\n");

	while (l != NULL) {
		odsadit();
		print_Symbol(l->a);
		l = next(l);
	}

	odsazeni--;
	return 0;
}


int print_Cons(Cons *c)
{
	if (c == NULL) { printf("NIL\n"); return 1; }
	if (type_match((t_point) c, NUMBER)) {
		printf("Jejda miso!\n");
		return 1;
	}

	odsazeni++;
	printf("[Cons \n");
	odsadit(); print_Symbol(c->a);
	odsadit(); print_Symbol(c->b);
	odsazeni--;
	odsadit(); printf("]\n");

	return 0;
}


int print_Function(Function *f)
{
	printf("Function:\n");
	if (f == NULL) return 1;
	odsazeni++;

	odsadit(); printf("Number of parameters: %d\n", f->params_count);

	if (f->built_in) {
		odsadit(); printf("Build in\n");
	} else {
#ifndef DEBUG
		odsadit(); printf("Not built in\n");
#else
		odsadit(); printf("Function body - "); print_Thunk(f->body.structure);
#endif
	}

	odsazeni--;
	return 0;
}


int print_Thunk(Thunk *t)
{
	printf("Thunk:\n");
	if (t == NULL) return 1;
	int tmp = resolve;
	odsazeni++;
	resolve = 0;

	odsadit(); print_Symbol(t->function);
	odsadit(); printf("Parameters - "); print_List(t->params);

	odsazeni--;
	resolve = tmp;
	return 0;
}
