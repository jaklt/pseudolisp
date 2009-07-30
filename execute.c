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


/**
 * Zjisti zda list l je aspon count dlouhy. Do other
 * nastavi odkaz na (count+1)-ty prvek.
 */
static int list_length_ok(Cons *l, int count, Cons **other)
{
	while (l != NULL && count) {
		count--; l = next(l);
	}

	*other = l;
	return count == 0;
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


t_point insert_params(Cons *params, Function *kam)
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

	l = ip_run(exp_params, kam->body.structure);

	return l;
}


static t_point result(Thunk *t, int *done)
{
	if (type_match(t->function, FUNCTION) && (t->function != BOOL_TRUE)) {
		Function *f = get_Func(t->function);
		Cons *other_params = NULL;

		if (list_length_ok(t->params, f->params_count, &other_params)) {
			*done = 0;

			// prebytek parametru
			if (f->more_params) other_params = NULL;

			if (f->built_in)
				t->function = f->body.link(t->params);
			else
				t->function = insert_params(t->params, f);

			t->params = other_params;
			if (other_params == NULL)
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

	// nejde ani o volani funkce ani o nevyhodnoceny Thunk
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
		if (!is_Thunk(s)) {
			if (akt == start) break;
			goto dalsi;
		}

		if (seen[akt]) {
			int zaloha = akt;
			filo[akt] = result(get_Thunk(s), &seen[akt]);

			// pokud se tohle pokazi tak je nekde neco hodne spatne
			if (zaloha != akt) ERROR(INNER_ERROR);

			// nic se nezmenilo
			if ((s == filo[akt]) && seen[akt]) {
				if (akt == start) break;
				akt--;
			}
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
