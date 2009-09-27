#ifndef PARSE_H
#define PARSE_H

#include "hashovani.h"
#include "structs.h"

#define OPEN_TAG  '['
#define CLOSE_TAG ']'
#define REMAIN_PARAMS_TAG '&'
#define PROMPT "~~> "
#define MAX_NAME_LENGTH 101

static inline int is_whitespace(const char c)
{
	return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

Hash *get_basic_hash();
void set_prompt(int set);
void set_quiet (int set);
void set_input(FILE *inp);

char read_char();
int read_word(char *chars, int if_remain);
t_point parse_char();
t_point parse_string();

void play();

#endif
