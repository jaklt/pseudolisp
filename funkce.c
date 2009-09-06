#include <stdlib.h>

#include "funkce.h"
#include "error.h"
#include "execute.h"
#include "gc.h"

#define resolve_and_store(s) ((s) = resolve_Thunk(s))


static t_point inner_reduce(
		t_point (),
		t_point (*overeni)(t_point (), t_point , t_point ),
		Cons *l
	);


/**
 * Operace se seznamy
 * ------------------
 */

t_point car(Cons *l)
{
	Cons *hl = get_Cons(resolve_and_store(l->a));

	if (hl == NULL) ERROR(TYPE_ERROR);
	return hl->a;
}


t_point cdr(Cons *l)
{
	Cons *ret = get_Cons(resolve_and_store(l->a));
	
	if (ret == NULL) ERROR(TYPE_ERROR);
	return ret->b;
}


t_point list(Cons *params)
{
	return make_Cons(params);
}


t_point cons(Cons *params)
{
	return pnew_Cons(params->a, next(params)->a);
}


t_point get_append()
{
	static Function *f = NULL;

	if (f == NULL) {
		f = new_Function(NIL, 2);

		f->built_in = 1;
		f->body.link = append;
		f->more_params = 1;

		gc_inc_immortal(make_Func(f));
	}

	return make_Func(f);
}


// XXX nestandartne dokaze pracovat i s prvky co nejsou Cons
t_point append(Cons *params)
{
	t_point s = resolve_and_store(params->a);
	Cons *c = next(params);

	if (is_NIL(s)) {
		if (c->b == NIL)
			return c->a;
		else
			return append(c);
	} else if (!type_match(s, CONS)) {
		if (c->b == NIL)
			return pnew_Cons(s, c->a);
		else
			return pnew_Cons(s, pnew_Thunk(get_append(), next(params)));
	} else if (type_match(s, CONS)) {
		c = get_Cons(s);
		return pnew_Cons(c->a, pnew_Thunk(get_append(), new_Cons(c->b, pnext(params))));
	} else
		ERROR(INNER_ERROR);
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
	return make_Bool(!is_NIL(vysl));
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
	t_point b = resolve_and_store(params->a);
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
		t = resolve_and_store(params->a) == BOOL_TRUE;
		params = next(params);
	}

	return make_Bool(t);
}


t_point op_or (Cons *params)
{
	int t = 0;

	while (params != NULL && !t) {
		t = resolve_and_store(params->a) == BOOL_TRUE;
		params = next(params);
	}

	return make_Bool(t);
}


t_point op_not(Cons *params)
{
	t_point s = resolve_and_store(params->a);

	if (!is_Bool(s)) ERROR(TYPE_ERROR);

	return make_Bool(!(s == BOOL_TRUE));
}


t_point op_nil(Cons *params)  { return make_Bool(is_NIL(resolve_and_store(params->a))); }
t_point op_cons(Cons *params) { return make_Bool(type_match(resolve_and_store(params->a), CONS)); }
t_point op_num(Cons *params)  { return make_Bool(type_match(resolve_and_store(params->a), NUMBER)); }

t_point op_bool(Cons *params)
{
	t_point s = resolve_and_store(params->a);
	return make_Bool(is_Bool(s));
}

t_point op_func(Cons *params)
{
	t_point s = resolve_and_store(params->a);
	return make_Bool(is_Func(s) || is_Thunk(s));
}


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
	t_point s = resolve_and_store(params->a);
	t_point s2 = resolve_and_store(next(params)->a);
	Cons *l = get_Cons(s2);

	if (is_NIL(s2)) return s;
	if (!is_Func(s) && !is_Thunk(s)) ERROR(TYPE_ERROR);

	// XXX - pfuj, ale resi neuplne definovane seznamy
	//     - neni mozne predat nekonecno parametru
	while (l != NULL) {
		if (!is_Cons(l->b)) l->b = resolve_Thunk(l->b);
		l = next(l);
	}

	return resolve_Thunk(pnew_Thunk(s, get_Cons(s2)));
}


static t_point inner_reduce(
		t_point (*operace)(void),
		t_point (*overeni)(t_point (*operace)(void), t_point, t_point),
		Cons *l
	)
{
	if (l == NULL) ERROR(INNER_ERROR);

	t_point s = resolve_and_store(l->a);
	l = next(l);

	while (l != NULL && s != NIL) {
		s = overeni(operace, resolve_and_store(s), resolve_and_store(l->a));
		l= next(l);
	}

	return s;
}
