#ifndef HELPER_H
#define HELPER_H

#include "structs.h"
#include "hashovani.h"

char *new_temp_name();
int print_Typ(E_TYPE t);
int print_Symbol(Symbol *s);
int print_List(List *l);
int print_Function(Function *f);
int print_Thunk(Thunk *t);
int print_Hash(Hash *h);
Symbol *print(List *params);

#endif
