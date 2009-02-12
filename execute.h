#ifndef EXEC_H
#define EXEC_H

#include "structs.h"


Symbol *resolve_Tank(Symbol *s);
Symbol *result(Function *f, List *params);
Symbol *call(List *l);

#endif
