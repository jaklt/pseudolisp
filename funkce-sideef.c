#include <stdio.h>

#include "error.h"
#include "execute.h"
#include "funkce.h"
#include "helpers.h"
#include "parser.h"


// TODO prepsat tak, ze vrati seznam nazvu promenych = nutno vypsat print-string
t_point env(Cons *params)
{
	Hash *h = get_basic_hash();

	for (int i=0; i<h->size; i++) {
		if (h->hashes[i].full != FULL_HASH) continue;
		printf("\"%s\" ", h->hashes[i].name);
	}
	printf("\n");

	return BOOL_TRUE;
}


/**
 * Ladici funkce. Vypise parametry a vrati prvni.
 */
t_point f_dump(Cons *params)
{
	Cons *l = params;
	while (l != NULL) {
		print_Symbol(l->a);
		l = next(l);
	}

	return params->a;
}


/**
 * Vypise parametry od nichz pozaduje, aby byly stringy
 * jako stringy.
 */
t_point f_print_string(Cons *params)
{
	Cons *l = params;
	Cons *la;
	t_point s;

	while (l != NULL) {
		s = resolve_Thunk(l->a);
		if (is_Cons(s)) {
			la = get_Cons(s);
			while (la != NULL) {
				la->a = resolve_Thunk(la->a);
				if (!is_Num(la->a))
					ERROR(TYPE_ERROR);

				putchar(get_Num(la->a));
				la = next(la);
			}
		}

		l = next(l);
	}

	printf("\n");
	return BOOL_TRUE;
}
