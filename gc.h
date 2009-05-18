#ifndef GC_H
#define GC_H

#include "structs.h"

int gc();
int gc_init();
int gc_score();
int gc_collect(E_TYPE t, void *p);
int gc_inc_immortal(E_TYPE t, void *p);
int gc_dec_immortal(E_TYPE t, void *p);

#endif
