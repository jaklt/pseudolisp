#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "execute.h"
#include "funkce.h"
#include "helpers.h"
#include "parser.h"
#include "hashovani.h"

#define TO_WRITE 1
#define TO_READ  2

static Hash *opened_files = NULL;
static int c2 = 0;

static Hash *get_opened_files()
{
	if (opened_files == NULL) opened_files = new_Hash();
	return opened_files;
}


static t_point rw_func(const int mode)
{
	static t_point w = NIL;
	static t_point r = NIL;

	if (mode == TO_READ && r == NIL)
		r = make_Func(new_inner_Func(1, 0, f_read));

	if (mode == TO_WRITE && w == NIL)
		w = make_Func(new_inner_Func(2, 0, f_write));

	return mode == TO_READ ? r : w;
}


// TODO prepsat tak, ze vrati seznam nazvu promenych = nutno vypsat print-string
t_point env(Cons *params)
{
	Hash *h = get_basic_hash();

	for (int i=0; i<h->size; i++) {
		if (h->hashes[i].full != FULL_HASH) continue;
		printf("\"%s\" ", h->hashes[i].name);
	}
	printf("\n");

	return BOOL_TRUE;
}


/**
 * Ladici funkce. Vypise parametry a vrati prvni.
 */
t_point f_dump(Cons *params)
{
	Cons *l = params;
	while (l != NULL) {
		print_Symbol(l->a);
		l = next(l);
	}

	return params->a;
}


static inline void to_output(Cons *la, FILE *fd)
{
	while (la != NULL) {
		la->a = resolve_Thunk(la->a);
		if (!is_Num(la->a)) ERROR_RET(TYPE_ERROR);

		putc(get_Num(la->a), fd);
		la->b = resolve_Thunk(la->b);
		la = next(la);
	}
}


/**
 * Vypise parametry od nichz pozaduje, aby byly stringy
 * jako stringy.
 */
t_point f_print_string(Cons *params)
{
	Cons *l = params;
	Cons *la;
	t_point s;

	while (l != NULL) {
		s = resolve_Thunk(l->a);
		if (is_Cons(s)) {
			la = get_Cons(s);
			to_output(la, stdout);
		} else
			ERROR(TYPE_ERROR);

		l = next(l);
	}

	return BOOL_TRUE;
}


static t_point open_file(t_point p, const int mode)
{
	Cons *l = get_Cons(resolve_Thunk(p));
	Hash *of = get_opened_files();
	HashMember *hm;
	FILE *f;
	t_point hash;

	char name[MAX_NAME_LENGTH]; // TODO jinak
	char *c = name;

	// nacteni jmena souboru
	while ((c - name) < MAX_NAME_LENGTH && l) {
		l->a = resolve_Thunk(l->a);
		*c++ = get_Num(l->a);
		l->b = resolve_Thunk(l->b);
		l = next(l);
	}
	*c = '\0';

	hash = hash_string(name);
	if (mode == TO_READ)
		// cist jeden soubor muzeme i vickrat najednou
		hash = p2n(hash + (++c2)*8161);
	else {
		hash = p2n(hash);
		if (get_Hash(of, hash)) ERROR(FILE_OPENED);
	}

	if (!(f = fopen(name, mode == TO_READ ? "r" : "w"))
			|| !(hm = add_Hash(of, hash, (t_point) f)))
		ERROR(CANNOT_OPEN_FILE);

	if (mode == TO_READ) {
		hm->name = malloc(sizeof(char) * MAX_NAME_LENGTH);
		hm->name[0] = '\0';
	}

	return make_Num(hash);
}


t_point read_open(Cons *params)
{
	t_point fd = open_file(params->a, TO_READ);
	return pnew_Cons(fd, pnew_Thunk(rw_func(TO_READ), new_List(fd)));
}


t_point write_open(Cons *params)
{
	t_point fd = open_file(params->a, TO_WRITE);
	return pnew_Cons(fd, pnew_Thunk(rw_func(TO_WRITE), new_List(fd)));
}


t_point f_close(Cons *params)
{
	Hash *of = get_opened_files();
	HashMember *hm = get_Hash(of, get_Num(resolve_Thunk(params->a)));
	if (hm == NULL) ERROR(FILE_NOT_OPENED);
	FILE *f = (FILE *) hm->link;

	fclose(f);
	if (hm->name != NULL) free(hm->name);
	del_HashMember(of, hm);
	return BOOL_TRUE;
}


t_point f_read(Cons *params)
{
	HashMember *hm = get_Hash(get_opened_files(), get_Num(resolve_Thunk(params->a)));
	if (hm == NULL) ERROR(FILE_NOT_OPENED);
	FILE *f = (FILE *) hm->link;

	// hm->name docteno -> treba nacist dalsi
	if (hm->info >= MAX_NAME_LENGTH || hm->name[hm->info] == '\0') {
		hm->name[hm->info = 0] = 0;
		if (fgets(hm->name, MAX_NAME_LENGTH, f) == NULL) return NIL;
	}

	return pnew_Cons(
			make_Num(hm->name[hm->info++]),
			pnew_Thunk(rw_func(TO_READ), new_List(make_Num(hm->hash))));
}


t_point f_write(Cons *params)
{
	HashMember *hm = get_Hash(get_opened_files(), get_Num(resolve_Thunk(params->a)));
	if (hm == NULL) ERROR(FILE_NOT_OPENED);
	
	to_output(get_Cons( resolve_Thunk(next(params)->a)), (FILE *) hm->link);
	return BOOL_TRUE;
}
