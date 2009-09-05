#include <stdio.h>
#include <stdlib.h>
#include "gc.h"
#include "hashovani.h"
#include "error.h"


static int counter = 0;
static int freed   = 0;

static Hash *hashes  = NULL;


void gc_init()
{
	if (hashes == NULL)
		hashes = new_Hash();

	return;
}


static inline HashMember *gc_inner_collect(t_point t)
{
	counter++;
	return add_Hash(hashes, t, t);
}


static int gc_immortal_change(t_point p, int change)
{
	HashMember *hm = get_Hash(hashes, p);

	if (hm == NULL)
		ERROR(INNER_ERROR);

	hm->info += change;
	return 0;
}


int gc_inc_immortal(t_point p) { return gc_immortal_change(p,  1); }
int gc_dec_immortal(t_point p) { return gc_immortal_change(p, -1); }
inline int gc_collect(t_point t) { return !gc_inner_collect(t); }


int gc_score()
{
	printf("%d collected, %d freed\n", counter, freed);
	return 0;
}


int gc()
{
	return 0;
}
