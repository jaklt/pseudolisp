#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "funkce.h"
#include "hashovani.h"
#include "parser.h"
#include "helpers.h"
#include "error.h"
#include "execute.h"


Hash *get_basic_hash()
{
	#define NUM_FUN sizeof(array_of_functions)/sizeof(struct function)

	Hash *h = new_Hash();
	Function *f;

	struct function {
		char *name;
		Symbol *(*link)(List *);
		int number_of_params;
	} array_of_functions[] = {
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

	for (int i=0; i<NUM_FUN; i++) {
		f = new_Function(NULL, array_of_functions[i].number_of_params);
		f->built_in = BOOL_TRUE;
		f->body.link = array_of_functions[i].link;

		add_Hash(h, array_of_functions[i].name, new_Symbol(FUNCTION, f));
	}

	add_Hash(h, "NIL", new_NIL());
	add_Hash(h, "TRUE",  new_Ordinal(BOOL, BOOL_TRUE));
	add_Hash(h, "FALSE", new_Ordinal(BOOL, BOOL_FALSE));

	return h;
}


static inline int is_whitespace(char c)
{
	switch (c) {
		case ' ':
		case '\n':
		case '\t':
		case EOF:
			return 1;
		default:
			return 0;
	}
}


int run_file(char *file_name)
{
	char c;
	while ((c = getchar()) != EOF);
	return 0;
}


int def(Hash *h, char *name);


Symbol *get_Undefined()
{
	static Symbol *u = NULL;

	if (u == NULL) {
		Function *f = new_Function(NULL, 0);
		f->built_in = BOOL_TRUE;
		f->body.link = undefined;

		u = new_Symbol(FUNCTION, f);
	}

	return u;
}


/**
 * Takhle to neni dobry, viz:
 *   [def b [] [+ a 3]]
 *   [def a [] 3]
 */
Symbol *create_token(Hash *h, char *string)
{
	Symbol *s = NULL;
	
	if (('0' <= string[0] && string[0] <='9')
			|| (string[0] == '-' && '0' <= string[1] && string[1] <='9'))
	{
		s = new_Ordinal(NUMBER, atof(string));
	}

	if (string[0] == '\'' || string[0] == '"')
		ERROR(NOT_IMPLEMENTED);

	if (s == NULL) {
		HashPrvek *hp = get_Hash(h, string);

		if (hp == NULL) {
			hp = add_Hash(h, string, get_Undefined());
		}

		s = hp->link;
	}

	return s;
}


List *parse_pipe(Hash *h)
{
	char chars[101];
	char *c = chars;
	int whitespaces = 1;
	int first = 1;
	int is_def = 0;
	int is_close_tag = 0;
	List *all = new_List(NULL);
	List *l = all;


	while ((*c = getchar()) != EOF) {
		if (is_whitespace(*c) || (is_close_tag = (*c == CLOSE_TAG))) {
			if (whitespaces) continue;

			*(c) = '\0';
			whitespaces = 1;

			if (strcmp(chars, "def") == 0 && first){
				first = 0; is_def = 1;
				continue;
			}

			if (c != chars) {
				l->next = new_List(create_token(h, chars));
				l = l->next;
			}

			if (is_close_tag) break;

			c = chars;
			first = 0;
			continue;
		}

		whitespaces = 0;

		if (is_def) {
			throw_error(NOT_IMPLEMENTED);
			break;
		}

		if (*c == OPEN_TAG) {
			l->next = new_List(new_Symbol(LIST, parse_pipe(h)));
			l = l->next;
		} else
			c++;
	}

	l = all->next; free(all);
	return l;
}


int play()
{
	Hash *h = get_basic_hash();
	char c;
//	print_Hash(h);

	while ((c = getchar()) != EOF) {
		if (c == OPEN_TAG) {
			print_Symbol(resolve_Thunk(call(parse_pipe(h))));
		//	print_List(parse_pipe(h));
			printf("\n---\n\n");
		}
	}

	return 0;
}

/*
TODO nejde:
[+ 1
2
]
*/
