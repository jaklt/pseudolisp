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


#define set_Hash(hp, NAME, INFO, FULL, HASH, LINK) {\
	hp.name = NAME; \
	hp.info = INFO; \
	hp.full = FULL; \
	hp.hash = HASH; \
	hp.link = LINK; }


static int empty_Hash(Hash *h)
{
	for (int i=0; i<(h->size); i++) {
		set_Hash(h->hashes[i], NULL, 0, EMPTY_HASH, 0, NULL);
	}

	return 0;
}


Hash *new_Hash()
{
	const int POC = 8;

	Hash *h = (Hash *) malloc(sizeof(Hash));

	h->hashes = (HashMember *) malloc(POC * sizeof(HashMember));
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


static int zvetsit_hash(Hash *h)
{
	HashMember *stare_hashes = h->hashes;
	int stara_size = h->size;

	h->size *= 2;
	h->hashes = (HashMember *) malloc((h->size)*sizeof(HashMember));
	if (h->hashes == NULL) return 1;
	empty_Hash(h);


	for (int i=0; i<stara_size; i++) {
		if (stare_hashes[i].full != EMPTY_HASH
			&& stare_hashes[i].hash != 0)
		{
			unsigned int index = volne_misto(h, stare_hashes[i].hash);
			set_Hash(h->hashes[index],
				stare_hashes[i].name,
				stare_hashes[i].info,
				stare_hashes[i].full, 
				stare_hashes[i].hash,
				stare_hashes[i].link
			);
		}
	}

	free(stare_hashes);
	return 0;
}


HashMember *add_Hash(Hash *h, char *name, void *p)
{
	h->used++;

	if (h->used > (3 * (h->size)/4))
		if (zvetsit_hash(h)) return NULL;

	unsigned long int hash = hash_string(name);
	unsigned int index = volne_misto(h, hash);

	set_Hash(h->hashes[index], name, 0, FULL_HASH, hash, p);

	return &h->hashes[index];
}


HashMember *get_Hash(Hash *h, char *s)
{
	unsigned long int hash = hash_string(s);
	unsigned int i = hash % h->size;

	while (h->hashes[i].full != EMPTY_HASH && h->hashes[i].hash != hash)
		i++;

	if (h->hashes[i].hash != hash) return NULL;

	return &h->hashes[i];
}


static HashMember *clone_HashMember(HashMember *puvodni, unsigned int size)
{
	HashMember *hp = malloc(size * sizeof(HashMember));

	for (int i=0; i<size; i++) {
		set_Hash(hp[i],
			puvodni[i].name,
			puvodni[i].info,
			puvodni[i].full,
			puvodni[i].hash, 
			puvodni[i].link
		);
	}


	return hp;
}


Hash *clone_Hash(Hash *puvodni)
{
	Hash *h = (Hash *) malloc(sizeof(Hash));

	h->hashes = clone_HashMember(puvodni->hashes, puvodni->size);
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
