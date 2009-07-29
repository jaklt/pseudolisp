#ifndef PARSE_H
#define PARSE_H

#include "hashovani.h"
#include "structs.h"

#define OPEN_TAG  '['
#define CLOSE_TAG ']'
#define REMAIN_PARAMS_TAG '&'
#define PROMPT "~~> "


Hash *get_basic_hash();
int set_prompt(int set);
int set_input(FILE *inp);

char read_char();
int read_word(char *chars, int if_remain);
t_point parse_char();
t_point parse_string();

int play();

#endif
