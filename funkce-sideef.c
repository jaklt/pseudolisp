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
		if (h->hashes[i].full == EMPTY_HASH) continue;
		printf("\"%s\" ", h->hashes[i].name);
	}
	printf("\n");

	return NULL;
}


Symbol *f_print(List *params)
{
	List *l = params;
	while (l != NULL) {
		print_Symbol(l->symbol);
		l = l->next;
	}

	return params != NULL ? params->symbol : NULL;
}


Symbol *f_print_string(List *params)
{
	List *l = params;
	Symbol *s;
	while (l != NULL) {
		s = resolve_Thunk(l->symbol);

		if (is_NIL(s)) ERROR(OPERACE_NEMA_SMYSL);

		if (s->type != CHAR) {
			if (s->type == LIST)
				f_print_string((List *)s->s.link);
			else
				ERROR(OPERACE_NEMA_SMYSL);
		} else 
			printf("%c", s->s.character);

		l = l->next;
	}
	printf("\n");

	return params != NULL ? params->symbol : NULL;
}
