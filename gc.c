#include <stdio.h>
#include <stdlib.h>
#include "gc.h"
#include "hashovani.h"

static int counter = 0;
static int freed   = 0;

static Hash **hashes = NULL;
static Hash *ihash   = NULL; // hash of imortals

#define p2num(p) ((unsigned long int) p)


int gc_init()
{
	if (hashes == NULL) {
		hashes = (Hash **) malloc(sizeof(Hash *)*4);

		hashes[NIL] = new_Hash();
		hashes[FUNCTION] = new_Hash();
		hashes[LIST] = new_Hash();
		hashes[THUNK] = new_Hash();
	}

	if (ihash == NULL)
		ihash = new_Hash();

	return 0;
}


static E_TYPE ok_type(E_TYPE t)
{
	if (t == FUNCTION || t == LIST || t == THUNK)
		return t;
	return NIL;
}


int gc_collect(E_TYPE t, void *p)
{
	t = ok_type(t);
	counter++;

	if (add_Hash(hashes[t], p2num(p), p))
		return 1;

	return 0;
}


int gc_immortal_change(E_TYPE t, void *p, int change)
{
	t = ok_type(t);
	HashMember *hm = get_Hash(hashes[t], p2num(p));
	HashMember *ihm; // immortal hash member

	if (hm != NULL) {
		ihm = get_Hash(ihash, p2num(p));
		if (ihm != NULL) {
			ihm->info += change;
		} else {
			ihm = add_Hash(ihash, p2num(p), p);
			ihm->info = change;
		}

		if (ihm->info == 0) {
			ihm->full = DELETED_HASH;
			ihash->used--;
		}
	} else
		return 1;

	return 0;
}


int gc_inc_immortal(E_TYPE t, void *p) { return gc_immortal_change(t, p,  1); }
int gc_dec_immortal(E_TYPE t, void *p) { return gc_immortal_change(t, p, -1); }


int gc_score()
{
	printf("%d collected, %d freed\n", counter, freed);
	return 0;
}


int gc()
{
	return 0;
}
