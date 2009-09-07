#ifndef HELPER_H
#define HELPER_H

#define min(a, b) ((a)<(b) ? (a) : (b))

#include "structs.h"

int print_Symbol(t_point s);
int print_List(Cons *c);
int print_Cons(Cons *c);
int print_Function(Function *f);
int print_Thunk(Thunk *t);

#endif
