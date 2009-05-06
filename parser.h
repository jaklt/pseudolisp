#ifndef PARSE_H
#define PARSE_H

#define OPEN_TAG  '['
#define CLOSE_TAG ']'
#define REMAIN_PARAMS_TAG '&'

#include "hashovani.h"

Hash *get_basic_hash();
int set_prompt(int set);
int set_input(FILE *inp);
int play();

#endif
