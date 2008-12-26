#ifndef HASHOVANI
#define HASHOVANI

#define PRAZDNY_HASH 0
#define PLNY_HASH   -1


// pri zmene treba zmenit clone
typedef struct SHashPrvek {
	char *jmeno;
	int plny:1;
	unsigned long int hash;
} HashPrvek;


typedef struct SHash {
	HashPrvek *pole;
	unsigned int velikost;
	unsigned int prvku;
} Hash;


Hash *new_Hash();
int add_hash(Hash *h, char *s);
HashPrvek *get_hash(Hash *h, char *s);
Hash *clone_Hash(Hash *h);
int delete_Hash(Hash *h);


#endif
