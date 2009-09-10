#include <stdio.h>
#include <stdlib.h>
#include "gc.h"
#include "hashovani.h"
#include "error.h"

#define check(hm) { if ((hm) != NULL && (hm)->info == 0 && hm->link == 0) \
                        to_free(hm); }

static unsigned long int collected = 0;
static unsigned long int freed   = 0;

static Hash *col  = NULL; // hash of collected
static void to_free(HashMember *hm);


void gc_init()
{
	if (col == NULL)
		col = new_Hash();

	return;
}


inline int gc_collect(t_point t)
{
	collected++;
	return !add_Hash(col, t, 0);
}


static HashMember *gc_change(t_point p, int change, void (*f)(HashMember *, int))
{
	if (is_Bool(p) || is_Num(p)) return NULL;
	HashMember *hm = get_Hash(col, p);
	if (hm == NULL)
		ERROR(INNER_ERROR);

	f(hm, change);
	return hm;
}

static void change_immort(HashMember *hm, int change) { hm->info += change; }
static void change_linked(HashMember *hm, int change) { hm->link += change; }

int gc_inc_immortal(t_point p) { return !gc_change(p,  1, change_immort); }
int gc_dec_immortal(t_point p) { return !gc_change(p, -1, change_immort); }


void gc_score()
{
	printf("%lu collected, %lu freed (%d left)\n", collected, freed, col->used);
}


static void to_change_linked(t_point s, int c)
{
	HashMember *hm;

	if (is_Func(s) && !get_Func(s)->built_in) {
		hm = gc_change(get_Func(s)->body.structure,  c, change_linked);
		check(hm);
	} else if (is_Cons(s) && !is_NIL(s)) {
		hm = gc_change(get_Cons(s)->a, c, change_linked);
		check(hm);
		hm = gc_change(get_Cons(s)->b, c, change_linked);
		check(hm);
	} else if (is_Thunk(s)) {
		hm = gc_change(get_Thunk(s)->function, c, change_linked);
		check(hm);
		hm = gc_change(make_Cons(get_Thunk(s)->params), c, change_linked);
		check(hm);
	}
}


static void to_free(HashMember *hm)
{
	t_point p = hm->hash;
	del_HashMember(col, hm);
	to_change_linked(p, -1);
	free((void *) p2n(p));
	freed++;
}


void gc()
{
	FOR_ALL_HASHES(col, hm, { to_change_linked(hm->hash, 1); })
	FOR_ALL_HASHES(col, hm, { check(hm); })
	FOR_ALL_HASHES(col, hm, { hm->link = 0; })
}
