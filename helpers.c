#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"
#include "execute.h"

static int odsazeni = 0;


static int odsadit()
{
	for (int i=0; i<odsazeni; i++)
		printf("  ");

	return 0;
}


int print_Typ(E_TYPE t)
{
	switch (t) {
		case FUNCTION: printf("Type: Function\n"); break;
		case   LIST: printf("Type: List\n"); break;
		case  THUNK: printf("Type: Thunk\n"); break;
		case    NIL: printf("Type: NIL\n"); break;
		case   BOOL: printf("Type: Boolean\n"); break;
		case   CHAR: printf("Type: Char\n"); break;
		case NUMBER: printf("Type: Number\n"); break;
		case PARAMETER: printf("Type: Parameter\n"); break;
		default:     printf("Wrong type (%i)\n", (int) t); return 1; 
	}

	return 0;
}


int print_Symbol(Symbol *s)
{
	s = resolve_Thunk(s); // TODO nejaky podmineny preklad/parameter?
	if (s == NULL) {
		printf("NIL - Empty\n");
		return 1;
	}

	switch (s->type) {
		case FUNCTION:
			printf("Function: built in %d, %d parameters\n", ((Function *)s->s.link)->built_in, ((Function *)s->s.link)->params_count);
			// print_Function((Function *) s->s.link);
			break;
		case  LIST: print_List((List *) s->s.link); break;
		case THUNK: print_Thunk((Thunk *) s->s.link); break;
		case   NIL: printf("NIL\n"); break;
		case  BOOL: printf("Bool: %s\n", (s->s.boolean == BOOL_TRUE) ? "TRUE" : "FALSE"); break;
		case  CHAR: printf("Char: '%c'\n", s->s.character); break;
		case NUMBER: printf("Number: %lli\n", s->s.number); break;
		case PARAMETER: printf("%d. parameter\n", (int) s->s.number); break;
		default: printf("Wrong value! (%u)\n", (unsigned int) s->type); break;
	}

	return 0;
}


int print_List(List *l)
{
	if (l == NULL) { printf("Empty List\n"); return 1; }
	odsazeni++;

	if (is_NIL(l->symbol)) {
		printf("List:\n");
		l = l->next;
	}
	else
		printf("Not just List:\n");

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

	odsadit(); printf("Number of parameters: %d\n", f->params_count);

	if (f->built_in) {
		odsadit(); printf("Build in\n");
	} else {
		odsadit(); printf("Not built in\n");
	//	odsadit(); printf("Function body - "); print_List(f->body.structure);
	}

	odsazeni--;
	return 0;
}


int print_Thunk(Thunk *t)
{
	printf("Thunk:\n");
	if (t == NULL) return 1;
	odsazeni++;

	odsadit(); print_Function(t->function);
	odsadit(); printf("Parameters - "); print_List(t->params);

	odsazeni--;
	return 0;
}


int print_Hash(Hash *h)
{
	printf("size: %d, used: %d\n", h->size, h->used);

	for (int i=0; i<h->size; i++) {
		if (h->hashes[i].full == EMPTY_HASH) continue;

		printf("\"%s\" ", h->hashes[i].name);
		print_Symbol((Symbol *)h->hashes[i].link);
	}

	return 0;
}


Symbol *f_print(List *params)
{
	List *l = params;
	while (l != NULL) {
		print_Symbol(l->symbol);
		l = l->next;
	}

	return params != NULL ? params->symbol : NULL;
}
