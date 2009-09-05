#ifndef GC_H
#define GC_H

#include "structs.h"

int gc();
void gc_init();
int gc_score();
int gc_collect(t_point p);
int gc_inc_immortal(t_point p);
int gc_dec_immortal(t_point p);

#endif
