#ifndef HELPER_H
#define HELPER_H

#include "structs.h"

char *new_temp_name();
int vypis_Typ(E_TYP t);
int vypis_Symbol(Symbol *s);
int vypis_List(List *l);
int vypis_Funkce(Funkce *f);
int vypis_Tank(Tank *t);

#endif
