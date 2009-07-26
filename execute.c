#include <stdlib.h>

#include "error.h"
#include "execute.h"


static Cons *f_append(Cons *a, Cons *b)
{
	if (a == NULL) return b;
	Cons *ret = new_Cons(a->a, NIL);
	Cons *l = ret;

	while (a->b != NIL) {
		a = next(a);
		l->b = pnew_Cons(a->a, NIL);
		l = next(l);
	}

	l->b = make_Cons(b);
	return l;
}


static int list_len(Cons *l)
{
	int i = 0;
	while (l != NULL) {
		i++; l = next(l);
	}

	return i;
}


/**
 * insert_params run
 */
static t_point ip_run(t_point *exp_params, t_point kam)
{
	if (kam == NIL) return NIL;

	else if (type_match(kam, CONS))
		return pnew_Cons(ip_run(exp_params, get_Cons(kam)->a),
		                ip_run(exp_params, get_Cons(kam)->b));

	else if (is_Param(kam))
		return exp_params[get_Num((t_point) get_Thunk(kam)->params) - 1];

	else if (type_match(kam, THUNK))
		return pnew_Thunk(ip_run(exp_params, get_Thunk(kam)->function),
		         get_Cons(ip_run(exp_params, make_Cons(get_Thunk(kam)->params))));

	return kam;
}


Thunk *insert_params(Cons *params, Function *kam)
{
	int count = kam->params_count + (kam->more_params ? 1 : 0);
	t_point exp_params[count];
	t_point l;

	for (int i=0; i<kam->params_count; i++) {
		exp_params[i] = params->a;
		params = next(params);
	}

	// doplneni posledniho parametru pro neomezeny pocet parametru
	if (kam->more_params)
		exp_params[count-1] = make_Cons(params);
//	else if(params != NULL) ERROR(TOO_MANY_PARAMS);

	l = ip_run(exp_params, make_Thunk(kam->body.structure));

	return get_Thunk(l);
}


static t_point result(Thunk *t, int *done)
{
	if (type_match(t->function, FUNCTION) && (t->function != BOOL_TRUE)) {
		Function *f = get_Func(t->function);
		if (list_len(t->params) >= f->params_count) {
			Cons *c = NULL;
			*done = 0;

			// prebytek parametru
			if (!f->more_params) {
				int i = f->params_count;
				c = t->params;
				while (i) { i--; c = next(c); }
			}

			if (f->built_in)
				t->function = f->body.link(t->params);
			else
				t->function = make_Thunk(insert_params(t->params, f));

			t->params = c;
			if (c == NULL)
				return t->function;
			else
				return make_Thunk(t);
		} else {
			*done = 1;
			return make_Thunk(t);
		}
	}
	
	if (type_match(t->function, THUNK) && !is_Param(t->function)) {
		*done = 0;
		Thunk *t2 = get_Thunk(t->function);
		t->function = t2->function;
		t->params = f_append(t2->params, t->params);

		return make_Thunk(t);
	}

	if (t->params == NULL)
		return t->function;
	else
		ERROR(TOO_MANY_PARAMS);
}


t_point resolve_Thunk(t_point s)
{
	static t_point filo[FILO_DEPTH];
	static int     seen[FILO_DEPTH];
	static int akt = 0;
	int start = akt;

	filo[akt] = s;
	seen[akt] = 0;

	while (1) {
		s = filo[akt];
		if (!type_match(s, THUNK) || is_Param(s)) {
			if (akt == start) break;
			goto dalsi;
		}

		if (seen[akt]) {
			int zaloha = akt;
			filo[akt] = result(get_Thunk(s), &seen[akt]);

			if (zaloha != akt) ERROR(UNDEFINED);

			// nic se nezmenilo
			if ((s == filo[akt]) && seen[akt]) akt--;
			continue;
		}

		// jeste jsme prvek nevideli && je to Thunk
		// -> dame funkci na zasobnik a vysledek vyhodnotime pozdeji
		else {
			seen[akt] = 1;
			if (++akt >= FILO_DEPTH) ERROR(TOO_DEEP_RECURSION);
			filo[akt] = get_Thunk(s)->function;
			seen[akt] = 0;
			continue;
		}

dalsi:
		if (--akt < start) ERROR(INNER_ERROR);
	}

	return filo[akt];
}
