#ifndef TESTS
#define TESTS

int test();

#include <stdio.h>
#include "structs.h"

int test()
{
	Cons *c = new_Cons(NIL, pnew_Cons(make_Num(12), NIL));
	Thunk *par = new_Param(1);
	Function *f = new_Function(par, 0);
	Thunk *t = new_Thunk(make_Func(f), NULL);

	printf("%lu\n", get_Num(next(c)->a));
	return 0;
}

#endif
