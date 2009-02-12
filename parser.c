#include <stdio.h>

#include "funkce.h"
#include "hashovani.h"


Hash *get_basic_hash()
{
	#define FUNKCI {sizeof(pole_funkci)/sizeof(struct function)}

	struct function {
		char *nazev;
		Symbol *(*link)(List *);
		int number_of_params;
	} pole_funkci[] = {
		{"+",		plus,	2},
		{"-",		minus,	2},
		{"*",		krat,	2},
		{"/",		deleno,	2},

		{"if",		op_if,	3},
		{"and",		op_and,	2},
		{"or",		op_or,	2},
		{"not",		op_not,	1},

		{"=",		eq,		2},
		{">",		gt,		2},

		{"head",	head,	1},
		{"tail",	tail,	1},
		{"take",	take,	2},

//		{"def",		def,	3},
		{"list",	list,	1},

		{"map",		map,	2},
		{"reduce",	reduce,	2},
		{"filter",	filter,	2},

		{"append",	append,	2},
		{"print",	print,	2},
	};

	return NULL;
}


static inline int is_whitespace(char c)
{
	switch (c) {
		case ' ':
		case '\n':
		case '\t':
			return 1;
		default:
			return 0;
	}
}


int run_file(char *jmeno_souboru[])
{
	char c;
	while ((c = getchar()) != EOF);
	return 0;
}


int play()
{
	char c;
	while ((c = getchar()) != EOF) {
		printf("%c = %d\n", c, (int)c);
	}

	return 0;
}
