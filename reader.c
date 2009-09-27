#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "error.h"
#ifndef READLINE
#define READLINE 0
#endif

#if READLINE
#define _FUNCTION_DEF
#include <readline/readline.h>
#include <readline/history.h>

#define IF_READLINE if (input != stdin) {
#define ELSE_READLINE(b) } else { b
#define ENDIF_READLINE }
#else
#define IF_READLINE
#define ELSE_READLINE(b)
#define ENDIF_READLINE
#endif

static char *readed_line = NULL;

#define READING_DONE -2
static int position = 0;


extern int prompt;
static FILE *input = NULL;

void set_input(FILE *inp)
{
	position = READING_DONE;
	input = inp;

	IF_READLINE
		if (readed_line == NULL)
			readed_line = malloc(sizeof(char) * MAX_NAME_LENGTH);
	ELSE_READLINE(
		if (readed_line != NULL) {
			free(readed_line); readed_line = NULL; })
	ENDIF_READLINE;
}


char read_char()
{
	if (position == EOF) return EOF;
	char c;
	
	if (position == READING_DONE) {
		IF_READLINE
			if (prompt) printf(PROMPT);
			if (fgets(readed_line, MAX_NAME_LENGTH, input) == NULL)
				return EOF;
		ELSE_READLINE(
			if ((readed_line = readline(prompt ? PROMPT : NULL)) == NULL)
				return EOF;)
		ENDIF_READLINE
		position = 0;
	}

	c = readed_line[position++];
	if (c == '\0' || c == EOF) {
		position = c == EOF ? EOF : READING_DONE;
		IF_READLINE
			if (c == '\0') return read_char();
		ELSE_READLINE(
			add_history(readed_line);
			free(readed_line);
			readed_line = NULL;
			if (c == '\0') return '\n'; )
		ENDIF_READLINE
	}

	if (c == '-' && readed_line[position] == '-') {
		position = READING_DONE;
		return read_char();
	}
	return c;
}


int read_word(char *chars, int if_remain)
{
	char *c = chars;

	while (is_whitespace(*c = read_char()));

	while (*c != OPEN_TAG && *c != CLOSE_TAG
			&& !is_whitespace(*(++c) = read_char())
			&& (c - chars) < MAX_NAME_LENGTH);

	if (*c == OPEN_TAG || (c - chars) >= MAX_NAME_LENGTH)
		ERROR(SYNTAX_ERROR);
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


inline t_point parse_char()
{
	return make_Num(parse_just_char(1));
}


t_point parse_string()
{
	Cons ret = {.b = NIL};
	Cons *l = &ret;
	char c;

	c = parse_just_char(0);
	while (c != '"' && c != '\0') {
		l->b = pnew_Cons(make_Num(c), NIL);
		l = next(l);
		c = parse_just_char(0);
	}

	if (c != '"') ERROR(SYNTAX_ERROR);
	return ret.b;
}
