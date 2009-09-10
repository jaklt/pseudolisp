#ifndef HASHOVANI
#define HASHOVANI

#define BASIC_HASH_SIZE 8


typedef enum {
	EMPTY_HASH,
	FULL_HASH,
	DELETED_HASH,
} HASH_TYPE;


// pri zmene treba zmenit clone
typedef struct SHashMember {
	char *name;
	int info;
	HASH_TYPE full;
	unsigned long int hash;
	unsigned long int link;
} HashMember;


typedef struct SHash {
	HashMember *hashes;
	unsigned int size;
	unsigned int used;
} Hash;


Hash *new_Hash();
HashMember *add_Hash(Hash *h, unsigned long int hash, unsigned long int p);
HashMember *add_string_Hash(Hash *h, char *name, unsigned long int p);

HashMember *get_Hash(Hash *h, unsigned long int hash);
#define get_string_Hash(h, name) get_Hash((h), hash_string(name))

Hash *del_HashMember(Hash *h, HashMember *hm);
#define del_Hash(h, hash) del_HashMember((h), get_Hash((h), (hash)))
#define del_string_Hash(h, name) del_Hash((h), hash_string(name))

Hash *clone_Hash(Hash *h);
void free_Hash(Hash *h);
unsigned long int hash_string(char *s);

#define FOR_ALL_HASHES(h, hm, body)\
	{\
		HashMember* hm; \
		for (int _i_=0; _i_<(h)->size; _i_++) { \
			if ((h)->hashes[_i_].full != FULL_HASH) continue; \
			hm = &((h)->hashes[_i_]); \
			body\
		}\
	}

#endif
