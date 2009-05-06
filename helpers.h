#ifndef HELPER_H
#define HELPER_H

#define min(a, b) ((a)<(b) ? (a) : (b))

#include "structs.h"

char *new_temp_name();
int print_Typ(E_TYPE t);
int print_Symbol(Symbol *s);
int print_List(List *l);
int print_Function(Function *f);
int print_Thunk(Thunk *t);

#endif
