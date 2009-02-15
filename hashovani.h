#ifndef HASHOVANI
#define HASHOVANI

#include "structs.h"


typedef enum {
	EMPTY_HASH,
	FULL_HASH,
} HASH_TYPE;


// pri zmene treba zmenit clone
typedef struct SHashPrvek {
	char *name;
	HASH_TYPE full;
	unsigned long int hash;
	Symbol *link;
} HashPrvek;


typedef struct SHash {
	HashPrvek *hashes;
	unsigned int size;
	unsigned int used;
} Hash;


Hash *new_Hash();
HashPrvek *add_Hash(Hash *h, char *name, Symbol *s);
HashPrvek *get_Hash(Hash *h, char *s);
Hash *clone_Hash(Hash *h);
int delete_Hash(Hash *h);


#endif
