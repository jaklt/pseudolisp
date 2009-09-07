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
HashMember *del_Hash(Hash *h, unsigned int hash);
HashMember *get_Hash(Hash *h, unsigned long int hash);

HashMember *add_string_Hash(Hash *h, char *name, unsigned long int p);
HashMember *del_string_Hash(Hash *h, char *name);
HashMember *get_string_Hash(Hash *h, char *name);

Hash *clone_Hash(Hash *h);
void free_Hash(Hash *h);

#endif
