#include <stdlib.h>
#include "structs.h"


Function *new_Function(t_point body_function, int params_count)
{
	Function *f = (Function *) malloc(sizeof(Function));

	f->body.structure = body_function;
	f->params_count   = params_count;
	f->built_in       = 0;
	f->more_params    = 0;

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
