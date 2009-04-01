#include "hashovani.h"
#include <stdio.h> // TODO smazat

static int counter = 0;


static Hash *get_gc_hash()
{
	static Hash *h = NULL;
	if (h == NULL) h = new_Hash();

	return h;
}


int collect(E_TYPE t, void *p)
{
	Hash *h = get_gc_hash();
	counter++;
	
	return 0;
}


int gc()
{
	Hash *h = get_gc_hash();
	printf("%d collected\n", counter);

	return 0;
}
