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


static int vyprazdnit_hash(Hash *h)
{
	for (int i=0; i<(h->velikost); i++) {
		h->pole[i].plny = PRAZDNY_HASH;
		h->pole[i].hash = 0;
	}

	return 0;
}


Hash *new_Hash()
{
	const int POC = 8;

	Hash *h = (Hash *) malloc(sizeof(Hash));

	h->pole = (HashPrvek *) malloc(POC * sizeof(HashPrvek));
	h->velikost = POC;
	h->prvku = 0;

	vyprazdnit_hash(h);

	return h;
}


static unsigned int volne_misto(Hash *h, unsigned long int hash)
{
	unsigned int index = (unsigned int) hash % h->velikost;

	while (h->pole[index].plny != PRAZDNY_HASH) {

		if (h->pole[index].hash == hash) {
			h->prvku--;
			return index;
		}

		index++;
		if (index >= (h->velikost)) index = 0;
	}

	return index;
}


//  memset (co, cim, kolikrat);
static int zvetsit_hash(Hash *h)
{
	HashPrvek *stare_pole = h->pole;
	int stara_velikost = h->velikost;

	h->velikost *= 2;
	h->pole = (HashPrvek *) malloc((h->velikost)*sizeof(HashPrvek));
	if (h->pole == NULL) return 1;
	vyprazdnit_hash(h);


	for (int i=0; i<stara_velikost; i++) {
		if (stare_pole[i].plny != PRAZDNY_HASH
			&& stare_pole[i].hash != 0)
		{
			unsigned int index = volne_misto(h, stare_pole[i].hash);
			h->pole[index].hash = stare_pole[i].hash;
			h->pole[index].jmeno = stare_pole[i].jmeno;
			h->pole[index].plny = PLNY_HASH;
		}
	}

	free(stare_pole);
	return 0;
}


int add_hash(Hash *h, char *s)
{
	h->prvku++;

	if (h->prvku > (3 * (h->velikost)/4))
		if (zvetsit_hash(h)) return 1;

	unsigned long int hash = hash_string(s);
	unsigned int index = volne_misto(h, hash);

	h->pole[index].hash = hash;
	h->pole[index].jmeno = s;
	h->pole[index].plny = PLNY_HASH;

	return 0;
}


HashPrvek *get_hash(Hash *h, char *s)
{
	unsigned long int hash = hash_string(s);
	unsigned int i = hash % h->velikost;

	while (h->pole[i].plny == PLNY_HASH && h->pole[i].hash != hash)
		i++;

	if (h->pole[i].hash != hash) return NULL;

	return &h->pole[i];
}


static HashPrvek *clone_HashPrvek(HashPrvek *puvodni, unsigned int velikost)
{
	HashPrvek *hp = malloc(velikost * sizeof(HashPrvek));

	for (int i=0; i<velikost; i++) {
		hp[i].jmeno = puvodni[i].jmeno;
		hp[i].plny = puvodni[i].plny;
		hp[i].hash = puvodni[i].hash;
	}


	return hp;
}


Hash *clone_Hash(Hash *puvodni)
{
	Hash *h = (Hash *) malloc(sizeof(Hash));

	h->pole = clone_HashPrvek(puvodni->pole, puvodni->velikost);
	h->velikost = puvodni->velikost;
	h->prvku = puvodni->prvku;

	return h;
}


int delete_Hash(Hash *h)
{
//	for (int i=0; i<h->velikost; i++) {
//		free(h->pole[i].jmeno);
//	}
	free(h->pole);
	free(h);

	return 0;
}
