#ifndef FUNKCE_H
#define FUNKCE_H

#include "structs.h"


Funkce **get_array_of_funtions();
Symbol *resolve_Tank(Symbol *s);


Symbol *map   (Funkce *f, List *l);
Symbol *reduce(Funkce *f, List *l);
Symbol *filter(Funkce *f, List *l);


/** Funkce pro podporu fungovani programu */
Symbol *call(Funkce *f, List *parametry);
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
Symbol *op_not(List *parametry);
Symbol *op_if (List *parametry);

/** Funkce pro praci s Listy */
Symbol *list(List *parametry);
Symbol *append(List *parametry);
Symbol *head(List *l);
Symbol *tail(List *l);
// Symbol *list_intersection(List *l); // prunik
// Symbol *list_union(List *l); // sjednoceni
#endif
