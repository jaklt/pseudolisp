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


static int empty_Hash(Hash *h)
{
	for (int i=0; i<(h->size); i++) {
		h->hashes[i].full = EMPTY_HASH;
		h->hashes[i].hash = 0;
		h->hashes[i].link = NULL;
	}

	return 0;
}


Hash *new_Hash()
{
	const int POC = 8;

	Hash *h = (Hash *) malloc(sizeof(Hash));

	h->hashes = (HashPrvek *) malloc(POC * sizeof(HashPrvek));
	h->size = POC;
	h->used = 0;

	empty_Hash(h);

	return h;
}


static unsigned int volne_misto(Hash *h, unsigned long int hash)
{
	unsigned int index = (unsigned int) hash % h->size;

	while (h->hashes[index].full != EMPTY_HASH) {

		if (h->hashes[index].hash == hash) {
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
	HashPrvek *stare_hashes = h->hashes;
	int stara_size = h->size;

	h->size *= 2;
	h->hashes = (HashPrvek *) malloc((h->size)*sizeof(HashPrvek));
	if (h->hashes == NULL) return 1;
	empty_Hash(h);


	for (int i=0; i<stara_size; i++) {
		if (stare_hashes[i].full != EMPTY_HASH
			&& stare_hashes[i].hash != 0)
		{
			unsigned int index = volne_misto(h, stare_hashes[i].hash);
			h->hashes[index].hash = stare_hashes[i].hash;
			h->hashes[index].name = stare_hashes[i].name;
			h->hashes[index].full = stare_hashes[i].full;
			h->hashes[index].link = stare_hashes[i].link;
		}
	}

	free(stare_hashes);
	return 0;
}


HashPrvek *add_Hash(Hash *h, char *name, Symbol *s)
{
	h->used++;

	if (h->used > (3 * (h->size)/4))
		if (zvetsit_hash(h)) return NULL;

	unsigned long int hash = hash_string(name);
	unsigned int index = volne_misto(h, hash);

	h->hashes[index].hash = hash;
	h->hashes[index].name = name;
	h->hashes[index].full = FULL_HASH;
	h->hashes[index].link = s;

	return &h->hashes[index];
}


HashPrvek *get_Hash(Hash *h, char *s)
{
	unsigned long int hash = hash_string(s);
	unsigned int i = hash % h->size;

	while (h->hashes[i].full != EMPTY_HASH && h->hashes[i].hash != hash)
		i++;

	if (h->hashes[i].hash != hash) return NULL;

	return &h->hashes[i];
}


static HashPrvek *clone_HashPrvek(HashPrvek *puvodni, unsigned int size)
{
	HashPrvek *hp = malloc(size * sizeof(HashPrvek));

	for (int i=0; i<size; i++) {
		hp[i].name = puvodni[i].name;
		hp[i].full = puvodni[i].full;
		hp[i].hash = puvodni[i].hash;
		hp[i].link = puvodni[i].link;
	}


	return hp;
}


Hash *clone_Hash(Hash *puvodni)
{
	Hash *h = (Hash *) malloc(sizeof(Hash));

	h->hashes = clone_HashPrvek(puvodni->hashes, puvodni->size);
	h->size = puvodni->size;
	h->used = puvodni->used;

	return h;
}


int delete_Hash(Hash *h)
{
//	for (int i=0; i<h->size; i++) {
//		free(h->hashes[i].name);
//	}
	free(h->hashes);
	free(h);

	return 0;
}
