#include <stdlib.h>
#include "helpers.h"

char *new_temp_name()
{
	static unsigned int i = 0;
	i++;

	char *c = malloc(sizeof(char)*7);
	c[0] = 't';
	c[1] = 'm';
	c[2] = 'p';

	c[3] = ((i / 100) % 10) + 48;
	c[4] = ((i / 10)  % 10) + 48;
	c[5] = ( i        % 10) + 48;

	c[6] = '\0';

	return c;
}
