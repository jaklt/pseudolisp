#ifndef STRUCT_H
#define STRUCT_H

#include "error.h"


typedef signed   long int t_number;
typedef unsigned long int t_point;

#define NIL       ((t_point) 0)

#define CONS      0
#define FUNCTION  1
#define THUNK     2
#define NUMBER    3

#define BOOL_TRUE  ((t_point) 1)
#define BOOL_FALSE ((t_point) 0) // zalezi zda chci, aby NIL == BOOL_FALSE

#define p2n(p) (((t_point) (p)) & (~(t_point)3))
#define type_match(p,t) (((p) & 3) == (t))
#define get_p(p,t) (type_match(p,t) ? (void *) (p) : ERROR_RET(TYPE_ERROR))
#define get_type(p) ((p) & 3)

#define is_NIL(p) ((p) == NIL)

#define make_Cons(p)  ((t_point) p2n(p)   | CONS)
#define make_Thunk(p) ((t_point) p2n(p)   | THUNK)
#define make_Func(p)  ((t_point) p2n(p)   | FUNCTION)
#define make_Num(p)   ((t_point) ((p) << 2) | NUMBER)

#define get_NIL(p)   (is_NIL(p) ? (void *) NULL : ERROR_RET(TYPE_ERROR))
#define get_Cons(p)  ((Cons *)     get_p(p, CONS))
#define get_Thunk(p) ((Thunk *)    get_p(p, THUNK))
#define get_Func(p)  ((Function *) get_p(p, FUNCTION))
#define get_Num(p)   (type_match(p,NUMBER) ? \
		(t_number) (p >> 2) : (t_number) ERROR_RET(TYPE_ERROR))

#define next(p)                  \
	(type_match((p)->b, CONS) ?  \
	 get_Cons((p)->b) : (Cons *) get_NIL((p)->b))


typedef struct SCons {
	t_point a;
	t_point b;
} Cons;


typedef struct SThunk {
	t_point function;
	Cons *params;
} Thunk;


typedef struct SFunction {
	unsigned int built_in : 1;
	unsigned int more_params : 1;
	int params_count;

	union {
		void *(*link)(Cons *);
		Thunk *structure;
	} body;
} Function;


Function *new_Function(Thunk *body_function, int params_count);
Thunk *new_Thunk(t_point fce, Cons *params);
Cons *new_Cons(t_point a, t_point b);
#define new_Param(a) new_Thunk(make_Num(a), NIL)

#define pnew_Function(a,b) make_Func(new_Function((a),(b)))
#define pnew_Thunk(a,b) make_Thunk(new_Thunk((a),(b)))
#define pnew_Cons(a,b) make_Cons(new_Cons((a),(b)))
#define pnew_Param(a) make_Thunk(new_Thunk(make_Num(a), NIL))

#endif
