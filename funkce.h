#ifndef FUNKCE_H
#define FUNKCE_H

#include "structs.h"


Function **get_array_of_funtions();

Symbol *map   (List *l);
Symbol *reduce(List *l);
Symbol *filter(List *l);


/** Function pro podporu fungovani programu */
// Symbol *def(List *params);
Symbol *print(List *params);
Symbol *undefined(List *params);

/** Function matematickych operaci */
Symbol *plus  (List *params);
Symbol *minus (List *params);
Symbol *krat  (List *params);
Symbol *deleno(List *params);

/** Function logickych operaci */
Symbol *op_and(List *params);
Symbol *op_or (List *params);
Symbol *op_not(List *params);

Symbol *op_if (List *params);

/** Function pro porovnavani */
Symbol *eq(List *params);
Symbol *gt(List *params);

/** Function pro praci s Listy */
Symbol *list(List *params);
Symbol *append(List *params);
Symbol *head(List *l);
Symbol *tail(List *l);
Symbol *take(List *l); // pr: (take 20 list)
// Symbol *list_intersection(List *l); // prunik
// Symbol *list_union(List *l); // sjednoceni
#endif
