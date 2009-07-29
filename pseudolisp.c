#include <stdio.h>
#include <stdlib.h>

#include "testy.h"
#include "parser.h"

#ifndef VERSION
#define VERSION "?.?.?"
#endif


static int prompt = 1;

int init()
{
	set_prompt(prompt);
	set_input(stdin);
//	set_show_collected(0);
	return 0;
}


int runargs(char *arg)
{
	switch (arg[0]) {
		case 't':
			test();
			exit(0);
		case 'v':
			printf("PseudoLISP " VERSION ", made by Jakl Tomas.\n");
			break;
		case 'c':
			init();
			play();
			break;
		case 'q':
			fprintf(stderr, "Not implemented yet.\n");
			break;
		case 'p':
			prompt = 0;
			break;
		case 'h': printf(
					"Usage: pseudolisp [options] [filenames]\n"
					"Options:\n"
					"  -v\tprint version\n"
					"  -t\trun tests\n"
					"  -c\trun console\n"
					"  -q\tquiet (only forced ouptut)\n"
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
			FILE *f = fopen(word, "r");
			set_prompt(0); set_input(f);
			play();
			fclose(f);
		}
	}

	return 0;
}
