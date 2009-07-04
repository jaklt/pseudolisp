#include <stdio.h>
#include "parser.h"
#include "error.h"


extern int prompt;
static FILE *input = NULL;

int set_input(FILE *inp)
{
	input = inp;
	return 0;
}


inline int is_whitespace(char c)
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

	if (c == '\n' && prompt) printf(PROMPT);
	return c;
}


int read_word(char *chars, int if_remain)
{
	char *c = chars;

	while (is_whitespace(*c = read_char()));

	while (*c != OPEN_TAG && *c != CLOSE_TAG
			&& !is_whitespace(*(++c) = read_char()));

	if (*c == OPEN_TAG) ERROR(SYNTAX_ERROR);
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


static char parse_just_char(int quoted)
{
	char c = read_char();

	if (quoted && c == '\'') {
		ERROR(SYNTAX_ERROR);
	} else if (c == '\\') {
		switch (read_char()) {
			case 't':  c = '\t'; break;
			case 'n':  c = '\n'; break;
			case '\n': c = '\n'; break;
			case '\\': c = '\\'; break;
			case '\'': c = '\''; break;
			case '"':  c = '"';  break;
			case '-':  c = '-';  break;
			default: ERROR(SYNTAX_ERROR);
		}
	}

	if (quoted && read_char() != '\'') ERROR(SYNTAX_ERROR);
	return c;
}


Symbol *parse_char()
{
	return new_Ordinal(CHAR, parse_just_char(BOOL_TRUE));
}


Symbol *parse_string()
{
	List *l = new_List(NULL);
	Symbol *ret = new_Symbol(LIST, l);
	char c;

	c = parse_just_char(BOOL_FALSE);
	while (c != '"' && c != '\0') {
		l->next = new_List(new_Ordinal(CHAR, c));
		l = l->next;
		c = parse_just_char(BOOL_FALSE);
	}

	if (c != '"') ERROR(SYNTAX_ERROR);
	return ret;
}
