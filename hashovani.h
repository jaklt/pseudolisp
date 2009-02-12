#ifndef HASHOVANI
#define HASHOVANI

#include "structs.h"

#define PRAZDNY_HASH 0
#define PLNY_HASH   -1


// pri zmene treba zmenit clone
typedef struct SHashPrvek {
	char *name;
	int full:1;
	unsigned long int hash;
	Function *function;
} HashPrvek;


typedef struct SHash {
	HashPrvek *pole;
	unsigned int size;
	unsigned int used;
} Hash;


Hash *new_Hash();
int add_hash(Hash *h, char *s, Function *f);
Function *get_hash(Hash *h, char *s);
Hash *clone_Hash(Hash *h);
int delete_Hash(Hash *h);


#endif
