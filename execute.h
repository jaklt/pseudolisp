#ifndef EXEC_H
#define EXEC_H

#include "structs.h"


Symbol *resolve_Thunk(Symbol *s);
Symbol *result(Function *f, List *params);

#endif
