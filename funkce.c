#include <stdlib.h>

#include "funkce.h"
#include "error.h"
#include "execute.h"


static t_point inner_reduce(
		t_point (),
		t_point (*overeni)(t_point (), t_point , t_point ),
		Cons *l
	);


/**
 * Operace se seznamy
 * ------------------
 */

t_point head(Cons *l)
{
	Cons *hl = get_Cons(resolve_Thunk(l->a));

	if (hl == NULL) ERROR(TYPE_ERROR);
	return hl->a;
}


// XXX mozna jeste nejaka kontrola typu nebo neco
t_point tail(Cons *l)
{
	Cons *ret = get_Cons(resolve_Thunk(l->a));
	
	if (ret == NULL) ERROR(TYPE_ERROR);
	return ret->b;
}


t_point list(Cons *params)
{
	return make_Cons(params);
}


t_point get_append()
{
	static Function *f = NULL;

	if (f == NULL) {
		f = new_Function(NULL, 2);

		f->built_in = 1;
		f->body.link = append;
		f->more_params = 1;

		//	gc_inc_immortal(???, make_Func(f));
	}

	return make_Func(f);
}


// XXX sezere False -> ma teda smysl mit oddeleny false od NIL,
//     nebo by append nemel pozirat NILy? (oboje divny)
// TODO jen nacrt
t_point append(Cons *params)
{
	/*
	t_point s = resolve_Thunk(params->a);
	Cons *c = next(params);

	if (is_NIL(s) || !type_match(s, CONS)) {
		if (c->b == NIL)
			return resolve_Thunk(c->a);
		else
			return append(c);
		return pnew_Cons(s, pnew_Thunk(get_append(), next(params)));
	}
	else if (type_match(s, CONS)) {
		c = get_Cons(s);

		return pnew_Cons(c->a, pnew_Thunk(get_append(), new_Cons(c->b, pnext(params))));
	}
	*/

	ERROR(NOT_IMPLEMENTED);
}


/**
 * Aritmeticke operace s cisly
 * ---------------------------
 */

t_point nubers_ok(t_point (*operace)(t_number, t_number), t_point a, t_point b);


static inline t_point f_plus  (t_number a, t_number b) { return make_Num(a+b); }
static inline t_point f_krat  (t_number a, t_number b) { return make_Num(a*b); }
static inline t_point f_minus (t_number a, t_number b) { return make_Num(a-b); }
static inline t_point f_deleno(t_number a, t_number b) { return make_Num(a/b); }


t_point plus  (Cons *params) { return inner_reduce(f_plus,   nubers_ok, params); }
t_point krat  (Cons *params) { return inner_reduce(f_krat,   nubers_ok, params); }
t_point minus (Cons *params) { return inner_reduce(f_minus,  nubers_ok, params); }
t_point deleno(Cons *params) { return inner_reduce(f_deleno, nubers_ok, params); }


t_point nubers_ok(t_point (*operace)(t_number, t_number), t_point a, t_point b)
{
	if (!is_Num(a) || !is_Num(b)) ERROR(TYPE_ERROR);
	return operace(get_Num(a), get_Num(b));
}


/**
 * Funkce pro porovnavani
 * ----------------------
 */

static inline t_point is_not_null(t_point vysl)
{
	if (!is_NIL(vysl))
		return BOOL_TRUE;
	else
		return BOOL_FALSE;
}


static inline t_point f_eq(t_number a, t_number b)
{
	return (a == b) ? make_Num(a) : NIL;
}


static inline t_point f_gt(t_number a, t_number b)
{
	return (a > b) ? make_Num(b) : NIL;
}


t_point eq(Cons *params)
{
	return is_not_null(inner_reduce(f_eq, nubers_ok, params));
}


t_point gt(Cons *params)
{
	return is_not_null(inner_reduce(f_gt, nubers_ok, params));
}


t_point op_if(Cons *params)
{
	t_point b = resolve_Thunk(params->a);
	if (!is_Bool(b)) ERROR(TYPE_ERROR);

	if (b == BOOL_TRUE)
		return next(params)->a;
	else
		return next(next(params))->a;
}


t_point op_and(Cons *params)
{
	int t = 1;

	while (params != NULL && t) {
		t = resolve_Thunk(params->a) == BOOL_TRUE;
		params = next(params);
	}

	return make_Bool(t);
}


t_point op_or (Cons *params)
{
	int t = 0;

	while (params != NULL && !t) {
		t = resolve_Thunk(params->a) == BOOL_TRUE;
		params = next(params);
	}

	return make_Bool(t);
}


t_point op_not(Cons *params)
{
	t_point s = resolve_Thunk(params->a);

	if (!is_Bool(s)) ERROR(TYPE_ERROR);

	return s == BOOL_TRUE ? BOOL_FALSE : BOOL_TRUE;
}


t_point op_nil(Cons *params)
{
	return make_Bool(is_NIL(resolve_Thunk(params->a)));
}


// TODO muze zpusobit pad
t_point op_list(Cons *params)
{
	return make_Bool(get_Cons(resolve_Thunk(params->a)) != NULL);
}


// TODO jinak
/*
static t_point op_ok(t_point s, int t)
{
	s = resolve_Thunk(s);
	return new_Ordinal(BOOL,
			!is_NIL(s) && s->type == t? BOOL_TRUE : BOOL_FALSE);
}


t_point op_num(Cons *params)  { return op_ok(params->symbol, NUMBER); }
t_point op_bool(Cons *params) { return op_ok(params->symbol, BOOL); }

t_point op_func(Cons *params)
{
	t_point s = resolve_Thunk(params->symbol);

	if (!is_NIL(s) && (s->type == FUNCTION || s->type == THUNK))
		return new_Ordinal(BOOL, BOOL_TRUE);
	else
		return new_Ordinal(BOOL, BOOL_FALSE);
}
*/


/**
 * next pomocne funkce
 * --------------------
 */

t_point undefined(Cons *params)
{
	ERROR(UNDEFINED);
}


t_point apply(Cons *params)
{
	t_point s = resolve_Thunk(params->a);

	if (!is_Func(s) && !is_Thunk(s))
		ERROR(TOO_MANY_PARAMS);

	t_point s2 = resolve_Thunk(next(params)->a);
	if (is_NIL(s2)) return s; // TODO zkusit pak oddelat

	return resolve_Thunk(pnew_Thunk(s, get_Cons(s2)));
}


static t_point inner_reduce(
		t_point (*operace)(void),
		t_point (*overeni)(t_point (*operace)(void), t_point, t_point),
		Cons *l
	)
{
	if (l == NULL) ERROR(INNER_ERROR);

	t_point s = resolve_Thunk(l->a);
	l = next(l);

	while (l != NULL && s != NIL) {
		s = overeni(operace, s, resolve_Thunk(l->a));
		l= next(l);
	}

	return s;
}
