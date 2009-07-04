#include <stdio.h>

#include "error.h"
#include "execute.h"
#include "funkce.h"
#include "helpers.h"
#include "parser.h"


// TODO prepsat tak, ze vrati seznam nazvu promenych = nutno vypsat print-string
Symbol *env(List *params)
{
	Hash *h = get_basic_hash();

	for (int i=0; i<h->size; i++) {
		if (h->hashes[i].full != FULL_HASH) continue;
		printf("\"%s\" ", h->hashes[i].name);
	}
	printf("\n");

	return NULL;
}


/**
 * Ladici funkce. Vypise parametry a vrati prvni.
 */
Symbol *f_print(List *params)
{
	List *l = params;
	while (l != NULL) {
		print_Symbol(l->symbol);
		l = l->next;
	}

	return params->symbol;
}


/**
 * Vypise parametry od nichz pozaduje, aby byly stringy
 * jako stringy.
 *
 * TODO rozhodnout se jeste co vracet
 */
Symbol *f_print_string(List *params)
{
	List *l = params;
	List *la;
	Symbol *s;

	while (l != NULL) {
		s = resolve_Thunk(l->symbol);
		if ((la = get_List(s)) != NULL) {
			while (la != NULL) {
				if (la->symbol->type != CHAR)
					ERROR(OPERACE_NEMA_SMYSL);

				putchar(la->symbol->s.character);
				la = la->next;
			}
		}

		l = l->next;
	}

	printf("\n");
	return new_Ordinal(BOOL, BOOL_TRUE);
}
