#ifndef PARSE_H
#define PARSE_H

#define OPEN_TAG  '['
#define CLOSE_TAG ']'


int parse(char *string[]);

int set_prompt(int set);
int set_input(FILE *inp);
int play();

#endif
