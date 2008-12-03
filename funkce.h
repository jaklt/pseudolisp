#ifndef FUNKCE_H
#define FUNKCE_H

#include "structs.h"


Funkce **get_array_of_funtions();
Symbol *operace_s_cisly(int (*operace)(int, int), List *l);


Symbol *map(Funkce *f, List *l);
Symbol *reduce(Funkce *f, List *l);
Symbol *filter(Funkce *f, List *l);


/** Funkce pro podporu fungovani programu */
Symbol *call(Funkce *f, List *parametry);
Symbol *run_main(/* ??????? */);
Symbol *def(char *jmeno, List *parametry);
Symbol *print(List *parametry);

/** Funkce matematickych operaci */
Symbol *plus  (List *parametry);
Symbol *minus (List *parametry);
Symbol *krat  (List *parametry);
Symbol *deleno(List *parametry);

/** Funkce logickych operaci */
Symbol *op_and(List *parametry);
Symbol *op_or (List *parametry);
Symbol *op_not(List *parametry); // jen jeden
Symbol *op_if (List *parametry);

#endif
