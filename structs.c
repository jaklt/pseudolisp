#include <stdlib.h>
#include "structs.h"


Function *new_Function(Thunk *body_function, int params_count)
{
	Function *f = (Function *) malloc(sizeof(Function));

	f->body.structure = body_function;
	f->params_count   = params_count;
	f->built_in       = BOOL_FALSE;
	f->more_params    = BOOL_FALSE;

	return f;
}


Cons *new_Cons(t_point a, t_point b)
{
	Cons *l = (Cons *) malloc(sizeof(Cons));

	l->a = a;
	l->b = b;

	return l;
}


Thunk *new_Thunk(t_point fce, Cons *params)
{
	Thunk *t = (Thunk *) malloc(sizeof(Thunk));

	t->function = fce;
	t->params = params;

	return t;
}
