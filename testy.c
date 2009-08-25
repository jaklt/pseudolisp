#include <stdio.h>

#include "structs.h"
#include "funkce.h"
#include "execute.h"
#include "helpers.h"


#define NEXT printf("\n-- next test:\n")
#define SPRAVNE(a) printf("\t** SPRAVNE: " a " **\n");
#define SPRAVNE_NUMBER(a,b) printf("\t** SPRAVNE: " a " **\n", b);


Function *getter(int params_count, t_point (*link)(Cons *))
{
	Function *f = new_Function(NIL, params_count);
	f->built_in = 1;
	f->body.link = link;

	return f;
}

Function *get_fplus()
{
	static Function *f = NULL;
	if (f == NULL) f = getter(2, plus);
	return f;
}


Function *get_fif()
{
	static Function *f = NULL;
	if (f == NULL) f = getter(3, op_if);
	return f;
}


Function *get_feq()
{
	static Function *f = NULL;
	if (f == NULL) f = getter(2, eq);
	return f;
}


t_point insert_params(Cons *params, Function *kam);
static int zakladni();
static int parametry();
static int resolvovani();
static int rekurze();
static int zakladni_fce();

int test()
{
//	gc_init();
	printf("-- Zacatek testovani:\n");
	zakladni(); NEXT;
	parametry(); NEXT;
	resolvovani(); NEXT;
	zakladni_fce(); NEXT;
	rekurze(); NEXT;

	return 0;
}


static int zakladni()
{
	Cons *c = new_Cons(NIL, pnew_Cons(make_Num(-12), NIL));
	Thunk *par = new_Param(1);
//	Function *f = get_fplus();
//	Thunk *t = new_Thunk(make_Func(f), NULL);

	printf("%li\n", get_Num(next(c)->a));
	SPRAVNE("-12");

	printf("Is param: %i\n", is_Param(make_Thunk(par)));
//	print_List(c);
//	print_Thunk(new_Thunk(make_Thunk(t), new_Cons(make_Num(12), NIL)));
//	print_Cons(c);
	return 0;
}


static int parametry()
{
	Function *f = get_fplus();
	t_point t = pnew_Thunk(make_Func(f), new_Cons(make_Num(5), pnew_Cons(pnew_Param(1), NIL)));
	Function *fce = new_Function(t, 1);
	Cons *l = new_List(make_Num(4));

//	print_Symbol(make_Func(fce));
	print_Symbol(insert_params(l, fce));
	SPRAVNE("9");
	return 0;
}


static int resolvovani()
{
	Function *f = get_fplus();
	t_point t = pnew_Thunk(make_Func(f), new_Cons(make_Num(5), pnew_Cons(pnew_Param(1), NIL)));
	Function *fce = new_Function(t, 1);
	Cons *l = new_List(make_Num(4));
	Thunk *go = new_Thunk(make_Func(fce), l);

	print_Symbol(resolve_Thunk(make_Thunk(go)));
	SPRAVNE("9");
	return 0;
}


static int zakladni_fce()
{
	Function *plus = get_fplus();
	Function *f_if = get_fif();
	Function *eq = get_feq();

	Thunk *t1 = new_Thunk(
			make_Func(plus),
			new_Cons(make_Num(1), pnew_List(make_Num(2))));

	Thunk *t2 = new_Thunk(
			make_Func(eq),
			new_Cons(make_Num(2), pnew_List(make_Thunk(t1))));

	Thunk *t3 = new_Thunk(
			make_Func(f_if),
			new_Cons(make_Thunk(t2), pnew_Cons(make_Num(1), pnew_List(make_Thunk(t1)))));

	print_Symbol(resolve_Thunk(make_Thunk(t3)));
	SPRAVNE("3");

	return 0;
}


/**
 * Suma
 */
static int rekurze()
{
	Function *plus = get_fplus();
	Function *eq = get_feq();
	Function *f_if = get_fif();
	Function *sum = new_Function(NIL, 1);

	Thunk *t1 = new_Thunk(make_Func(eq), new_Cons(make_Num(1), pnew_List(pnew_Param(1))));
	Thunk *t2 = new_Thunk(make_Func(plus), new_Cons(make_Num(-1), pnew_List(pnew_Param(1))));
	Thunk *t3 = new_Thunk(make_Func(sum), new_List(make_Thunk(t2)));
	Thunk *t4 = new_Thunk(make_Func(plus), new_Cons(pnew_Param(1), pnew_List(make_Thunk(t3))));

	Thunk *t = new_Thunk(
			make_Func(f_if),
			new_Cons(make_Thunk(t1), pnew_Cons(make_Num(1), pnew_List(make_Thunk(t4)))));
	sum->body.structure = make_Thunk(t);

	Thunk *go = new_Thunk(make_Func(sum), new_List(make_Num(20)));
	print_Symbol(resolve_Thunk(make_Thunk(go)));
	SPRAVNE("210");

	return 0;
}
