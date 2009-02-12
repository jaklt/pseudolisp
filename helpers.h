#ifndef HELPER_H
#define HELPER_H

#include "structs.h"

char *new_temp_name();
int print_Typ(E_TYP t);
int print_Symbol(Symbol *s);
int print_List(List *l);
int print_Function(Function *f);
int print_Tank(Tank *t);

#endif
