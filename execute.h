#ifndef EXEC_H
#define EXEC_H

#include "structs.h"


Symbol *resolve_Tank(Symbol *s);
Symbol *result(Funkce *f, List *parametry);
Symbol *call(List *l);

#endif
