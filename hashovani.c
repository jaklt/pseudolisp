#include <stdlib.h>
#include <string.h>

#include "hashovani.h"


/**
 * djb2 string hashing alorithm
 *
 * http://www.cse.yorku.ca/~oz/hash.html
 */
static unsigned long int hash_string(char *s)
{
	unsigned long int h = 5381;
	char *c = s;

	while (*c != '\0') {
		/* hash * 33 + c */
		h = ((h << 5) + h) + *c;
		c++;
	}

	return h;
}


static int empty_hash(Hash *h)
{
	for (int i=0; i<(h->size); i++) {
		h->pole[i].full = PRAZDNY_HASH;
		h->pole[i].hash = 0;
	}

	return 0;
}


Hash *new_Hash()
{
	const int POC = 8;

	Hash *h = (Hash *) malloc(sizeof(Hash));

	h->pole = (HashPrvek *) malloc(POC * sizeof(HashPrvek));
	h->size = POC;
	h->used = 0;

	empty_hash(h);

	return h;
}


static unsigned int volne_misto(Hash *h, unsigned long int hash)
{
	unsigned int index = (unsigned int) hash % h->size;

	while (h->pole[index].full != PRAZDNY_HASH) {

		if (h->pole[index].hash == hash) {
			h->used--;
			return index;
		}

		index++;
		if (index >= (h->size)) index = 0;
	}

	return index;
}


//  memset (co, cim, kolikrat);
static int zvetsit_hash(Hash *h)
{
	HashPrvek *stare_pole = h->pole;
	int stara_size = h->size;

	h->size *= 2;
	h->pole = (HashPrvek *) malloc((h->size)*sizeof(HashPrvek));
	if (h->pole == NULL) return 1;
	empty_hash(h);


	for (int i=0; i<stara_size; i++) {
		if (stare_pole[i].full != PRAZDNY_HASH
			&& stare_pole[i].hash != 0)
		{
			unsigned int index = volne_misto(h, stare_pole[i].hash);
			h->pole[index].hash = stare_pole[i].hash;
			h->pole[index].name = stare_pole[i].name;
			h->pole[index].full = PLNY_HASH;
			h->pole[index].function = stare_pole[i].function;
		}
	}

	free(stare_pole);
	return 0;
}


int add_hash(Hash *h, char *s, Function *f)
{
	h->used++;

	if (h->used > (3 * (h->size)/4))
		if (zvetsit_hash(h)) return 1;

	unsigned long int hash = hash_string(s);
	unsigned int index = volne_misto(h, hash);

	h->pole[index].hash = hash;
	h->pole[index].name = s;
	h->pole[index].full = PLNY_HASH;
	h->pole[index].function = f;

	return 0;
}


Function *get_hash(Hash *h, char *s)
{
	unsigned long int hash = hash_string(s);
	unsigned int i = hash % h->size;

	while (h->pole[i].full == PLNY_HASH && h->pole[i].hash != hash)
		i++;

	if (h->pole[i].hash != hash) return NULL;

	return h->pole[i].function;
}


static HashPrvek *clone_HashPrvek(HashPrvek *puvodni, unsigned int size)
{
	HashPrvek *hp = malloc(size * sizeof(HashPrvek));

	for (int i=0; i<size; i++) {
		hp[i].name = puvodni[i].name;
		hp[i].full = puvodni[i].full;
		hp[i].hash = puvodni[i].hash;
		hp[i].function = puvodni[i].function;
	}


	return hp;
}


Hash *clone_Hash(Hash *puvodni)
{
	Hash *h = (Hash *) malloc(sizeof(Hash));

	h->pole = clone_HashPrvek(puvodni->pole, puvodni->size);
	h->size = puvodni->size;
	h->used = puvodni->used;

	return h;
}


int delete_Hash(Hash *h)
{
//	for (int i=0; i<h->size; i++) {
//		free(h->pole[i].name);
//	}
	free(h->pole);
	free(h);

	return 0;
}
