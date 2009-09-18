#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
extern void test();

#ifndef VERSION
#define VERSION "?.?.?"
#endif


static int prompt = 1;
static int cmd = 0;

void init()
{
	set_prompt(prompt);
	set_input(stdin);
}


void runargs(char *arg)
{
	switch (arg[0]) {
		case 't':
			test();
			exit(0);
		case 'v':
			printf("PseudoLISP " VERSION ", made by Jakl Tomas.\n");
			break;
		case 'c':
			cmd = 1;
			break;
		case 'q':
			set_quiet(1);
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
			if (f == NULL) {
				fprintf(stderr, "Cannot read file '%s'\n", word);
				continue;
			}
			set_prompt(0); set_input(f);
			play();
			fclose(f);
		}
	}

	if (cmd) { init(); play(); }
	return 0;
}
