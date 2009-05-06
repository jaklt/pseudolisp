#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "funkce.h"
#include "parser.h"
#include "helpers.h"
#include "error.h"
#include "execute.h"
#include "gc.h"


List *parse_pipe(Hash *h, int level);


static int prompt = 1;

int set_prompt(int set)
{
	prompt = set;
	return 0;
}


static FILE *input = NULL;

int set_input(FILE *inp)
{
	input = inp;
	return 0;
}


Hash *get_basic_hash()
{
	#define NUM_FUN sizeof(array_of_functions)/sizeof(struct function)

	static Hash *h = NULL;
	if (h != NULL) return h;

	h = new_Hash();
	Function *f;

	struct function {
		char *name;
		Symbol *(*link)(List *);
		int params_count;
	} array_of_functions[] = {
	//	name	function	params
		{"+",		plus,	2},
		{"-",		minus,	2},
		{"*",		krat,	2},
		{"/",		deleno,	2},

		{"if",		op_if,	3},
		{"and",		op_and,	2},
		{"or",		op_or,	2},
		{"not",		op_not,	1},

		{"nil?",	op_nil,	1},
		{"list?",	op_list,1},
		{"number?",	op_num,	1},
		{"char?",	op_char,1},
		{"bool?",	op_bool,1},
		{"func?",	op_func,1},

		{"=",		eq,		2},
		{">",		gt,		2},

		{"head",	head,	1},
		{"tail",	tail,	1},
		{"append",	append,	2},

		{"list",	list,	1},
		{"print",	f_print,1},
		{"print-string",	f_print_string,1},
		{"env",		env,	0},
		{"apply",	apply,	2},
	};

	for (int i=0; i<NUM_FUN; i++) {
		f = new_Function(NULL, array_of_functions[i].params_count);
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


char read_char()
{
	static int is_prev = 0;
	static char prev;
	char c;
	if (input == NULL) ERROR(VNITRNI_CHYBA);

	if (is_prev) {
		c = prev;
		is_prev = 0;
	} else {
		c = getc(input);

		if (c == '-') {
			c = getc(input);

			if (c == '-') {
				while ((c = getc(input)) != '\n' && c != '\0');
			} else {
				prev = c;
				is_prev = 1;
				c = '-';
			}
		}
	}

	if (c == '\n' && prompt) printf("~~> ");
	return c;
}


static int read_word(char *chars, int if_remain)
{
	char *c = chars;

	while (is_whitespace(*c = read_char()));

	while (*c != OPEN_TAG && *c != CLOSE_TAG
			&& !is_whitespace(*(++c) = read_char()));

	if (*c == OPEN_TAG) throw_error(SYNTAX_ERROR);
	if (*c == CLOSE_TAG) {
		*c = '\0';
		return 0;
	}

	*c = '\0';

	if (if_remain && chars[0] == REMAIN_PARAMS_TAG) {
		while ((*c = read_char()) != CLOSE_TAG)
			if (!is_whitespace(*c)) ERROR(SYNTAX_ERROR);

		return 0;
	}

	return 1;
}


Symbol *init_def(Hash *h, char *name, int level)
{
	Function *f = new_Function(NULL, 0);
	Symbol *s = new_Symbol(FUNCTION, f);
	HashMember *hm = NULL;
	if (name != NULL) {
		hm = add_Hash(h, name, s);
		hm->info = level;
	}

	Hash *new_h = clone_Hash(h);
	char chars[101];
	int param_counter = level;

	// nacteni parametru funkce
	while (is_whitespace(chars[0] = read_char()));
	if (chars[0] != OPEN_TAG) ERROR(SYNTAX_ERROR);

	while (read_word(chars, BOOL_TRUE)) {
		add_Hash(new_h, chars, new_Ordinal(PARAMETER, ++param_counter));
	}

	if (chars[0] != '\0') {
		add_Hash(new_h, chars, new_Ordinal(PARAMETER, ++param_counter));

		// nastaveni parametru se zbytkem z volani
		if (chars[0] == REMAIN_PARAMS_TAG) {
			f->more_params = BOOL_TRUE;
			param_counter--;
		}
	}

	// asociace funkce
	f->built_in = BOOL_FALSE;
	f->params_count = param_counter;

	// vytvoreni tela funkce
	f->body.structure = parse_pipe(new_h, param_counter+f->more_params);
	if (f->body.structure == NULL) ERROR(SYNTAX_ERROR);
	delete_Hash(new_h);

	return s;
}


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


static Symbol *kontext_params(Symbol *function, int level)
{
	if (level <= 0) return function;

	List *l = new_List(function);
	function = new_Symbol(LIST, l);

	for (int i=1; i<=level; i++) {
		l->next = new_List(new_Ordinal(PARAMETER, i));
		l = l->next;
	}

	return function;
}


/**
 * TODO Takhle to neni dobry, viz:
 *   [def b [] [+ a 3]]
 *   [def a [] 3]
 */
Symbol *create_token(Hash *h, char *string, int level)
{
	Symbol *s = NULL;
	
	if (('0' <= string[0] && string[0] <='9')
			|| (string[0] == '-' && '0' <= string[1] && string[1] <='9'))
	{
		s = new_Ordinal(NUMBER, (t_number) atof(string));
	}

	if (string[0] == '\'') { // char
		// is ok?
		if (string[1] == '\0' || string[2] != '\'' || string[3] != '\0')
			ERROR(SYNTAX_ERROR);

		s = new_Ordinal(CHAR, string[1]);

	} else if (string[0] == '"') { // string
		List *l = new_List(NULL);
		s = new_Symbol(LIST, l);
		int i = 1;

		for (; string[i] != '"' && string[i] != '\0'; i++) {
			l->next = new_List(new_Ordinal(CHAR, string[i]));
			l = l->next;
		}

		if (string[i] != '"' || string[i+1] != '\0')
			ERROR(SYNTAX_ERROR);
	}

	if (s == NULL) {
		HashMember *hp = get_Hash(h, string);

		if (hp == NULL) {
			hp = add_Hash(h, string, get_Undefined());
		}

		s = hp->link;

		// doplneni kontextu do volani funkce
		if (s->type == FUNCTION)
			s = kontext_params(s, min(hp->info, level));
	}

	return s;
}


List *parse_pipe(Hash *h, int level)
{
	char chars[101];
	char *c = chars;
	int whitespaces = 1;
	int first = 1;
	int is_def = 0;
	int is_close_tag = 0;
	List *all = new_List(NULL);
	List *l = all;


	while ((*c = read_char()) != EOF) {
		if (is_whitespace(*c) || (is_close_tag = (*c == CLOSE_TAG))) {
			if (whitespaces && !is_close_tag) continue;

			*c = '\0';
			whitespaces = 1;

			if (first && ((is_def = (strcmp(chars, "def") == 0))
						|| (strcmp(chars, "lambda") == 0)))
			{
				if (is_close_tag) ERROR(SYNTAX_ERROR);
				if (is_def) {
					// is def
					if (!read_word(chars, BOOL_FALSE)) ERROR(SYNTAX_ERROR);
					init_def(h, chars, level);
				} else {
					// is lambda
					l->next = new_List(kontext_params(init_def(h, NULL, level), level));
					l = l->next;
				}

				break;
			}

			if (c != chars) {
				l->next = new_List(create_token(h, chars, level));
				l = l->next;
			}

			if (is_close_tag) break;

			c = chars;
			first = 0;
			continue;
		}

		whitespaces = 0;

		if (*c == OPEN_TAG) {
			l->next = parse_pipe(h, level);

			if (l->next != NULL) {
				l->next = new_List(new_Symbol(LIST, l->next));
				l = l->next;
			}
		} else
			c++;
	}

	l = all->next; free(all);
	return l;
}


int play()
{
	Hash *h = get_basic_hash();
	List *parsed;
	char c;

	if (prompt) printf("~~> ");
	while ((c = read_char()) != EOF) {
		if (c == OPEN_TAG) {
			parsed = parse_pipe(h, 0);
			if (parsed != NULL) {
				print_Symbol(resolve_Thunk(new_Symbol(LIST, parsed)));
			} else if (prompt)
				printf("OK\n");
		//	gc();
		}
	}

	printf("\n\n"); gc();
	return 0;
}
