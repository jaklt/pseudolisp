#ifndef FUNKCE_H
#define FUNKCE_H

#include "structs.h"


t_point apply(Cons *params);

/** Pomocne funkce */
t_point undefined(Cons *params);

/** Function matematickych operaci */
t_point op_plus (Cons *params);
t_point op_minus(Cons *params);
t_point op_mult (Cons *params);
t_point op_div  (Cons *params);

/** Function logickych operaci */
t_point op_and(Cons *params);
t_point op_or (Cons *params);
t_point op_not(Cons *params);
t_point op_if (Cons *params);

t_point op_nil (Cons *params);
t_point op_cons(Cons *params);
t_point op_num (Cons *params);
t_point op_char(Cons *params);
t_point op_bool(Cons *params);
t_point op_func(Cons *params);

/** Function pro porovnavani */
t_point op_gt(Cons *params);
t_point op_ge(Cons *params);
t_point op_eq(Cons *params);
t_point op_le(Cons *params);
t_point op_lt(Cons *params);

/** Funkce pro praci s dvojici */
t_point list(Cons *params);
t_point cons(Cons *params);
t_point car(Cons *l);
t_point cdr(Cons *l);
t_point append(Cons *params);

/** Sideefectove funkce */
t_point env(Cons *params);
t_point f_dump(Cons *params);
t_point f_print_string(Cons *params);

#endif
