#include <stdio.h> // TODO smazat
#include "gc.h"
#include "hashovani.h"

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
	printf("%d collected, %d freed\n", counter, 0);

	return 0;
}
