#include <stdio.h>
#include <stdlib.h>

#include "testy.h"
#include "parser.h"

int init()
{
	set_prompt(1);
//	set_show_collected(0);
//	test(); exit(0);
	return 0;
}


int runargs(char *arg)
{
	switch (arg[0]) {
		case 't':
			test();
			exit(0);
		case 'v':
			printf("PseudoLISP 0.0.4, made by JackeLee.\n");
			break;
		case 'c':
			play();
			break;
		case 'p':
			set_prompt(0);
			break;
		case 'h': printf(
					"Usage: pseudolisp [options] [filename]\nOptions:\n"
					"  -v\tprint version\n"
					"  -t\trun tests\n"
					"  -c\trun console\n"
					"  -p\tdisable prompt\n"
					"  -h\tprint this help\n\n");
			break;
		default:
			fprintf(stderr, "Invalid argument. Use -h for help.\n");
			exit(1);
			break;
	}

	if (*(++arg) != '\0') runargs(arg);
	return 0;
}


int main(int argc, char *argv[])
{
	init();
	if (argc == 1) play();

	char *word;
	for (int i=1; i<argc; i++) {
		word = argv[i];
		if (word[0] == '-') runargs(++word);
		else {
			fprintf(stderr, "Not implemented yet.\n");
			set_prompt(0);
			// stdin -> FILE
			exit(1);
		}
	}

	return 0;
}
