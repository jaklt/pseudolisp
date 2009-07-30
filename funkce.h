#ifndef FUNKCE_H
#define FUNKCE_H

#include "structs.h"


t_point apply(Cons *params);

/** Pomocne funkce */
t_point undefined(Cons *params);
Cons *get_List(t_point s);

/** Function matematickych operaci */
t_point plus  (Cons *params);
t_point minus (Cons *params);
t_point krat  (Cons *params);
t_point deleno(Cons *params);

/** Function logickych operaci */
t_point op_and(Cons *params);
t_point op_or (Cons *params);
t_point op_not(Cons *params);
t_point op_if  (Cons *params);

t_point op_nil (Cons *params);
t_point op_cons(Cons *params);
t_point op_num (Cons *params);
t_point op_char(Cons *params);
t_point op_bool(Cons *params);
t_point op_func(Cons *params);

/** Function pro porovnavani */
t_point eq(Cons *params);
t_point gt(Cons *params);

/** Function pro praci s Listy */
t_point list(Cons *params);
t_point append(Cons *params);
t_point head(Cons *l);
t_point tail(Cons *l);

/** Sideefectove funkce */
t_point env(Cons *params);
t_point f_dump(Cons *params);
t_point f_print_string(Cons *params);

#endif
