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
#define BOOL_FALSE ((t_point) 0) // --> NIL == BOOL_FALSE

#define MAX_NUM ((t_number) 0x1FFFFFFF)
#define MIN_NUM ((t_number) 0xE0000000)

#define p2n(p) (((t_point) (p)) & (~(t_point)3))
#define type_match(p,t) (((p) & 3) == (t))
#define get_p(p,t) (type_match(p,t) ? (void *) p2n(p) : ERROR_RET(TYPE_ERROR))
#define get_type(p) ((p) & 3)

#define make_Cons(p)  ((t_point) p2n(p)   | CONS)
#define make_Thunk(p) ((t_point) p2n(p)   | THUNK)
#define make_Func(p)  ((t_point) p2n(p)   | FUNCTION)
#define make_Bool(t)  ((t) ? BOOL_TRUE : BOOL_FALSE)
static inline t_point make_Num(t_number num)
{
	if (num >= MAX_NUM && num <= MIN_NUM)
		ERROR(INT_OVERFLOW);
	return (t_point) (num << 2 | NUMBER);
}

#define get_NIL(p)   (is_NIL(p) ? (void *) NULL : ERROR_RET(TYPE_ERROR))
#define get_Cons(p)  ((Cons *)     get_p(p, CONS))
#define get_Thunk(p) ((Thunk *)    get_p(p, THUNK))
#define get_Func(p)  ((Function *) get_p(p, FUNCTION))
#define get_Num(p)   (type_match(p,NUMBER) ? \
		(t_number) ((p) >> 2) | ((p) & 0x80000000 ? 0xC0000000 : 0x00000000) \
		: (t_number) ERROR_RET(TYPE_ERROR))

extern t_point resolve_Thunk(t_point p);
#define next(p)                  \
	(type_match((p)->b, CONS) ?  \
	 get_Cons((p)->b) : get_Cons(resolve_Thunk((p)->b)))

#define pnext(p)  make_Cons(next(p))

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
	unsigned int params_count;

	union {
		t_point (*link)(Cons *);
		t_point structure;
	} body;
} Function;


Function *new_Function(t_point body_function, int params_count);
Function *new_inner_Func(int params_count, int more_params, t_point (*link)(Cons *));
Thunk *new_Thunk(t_point fce, Cons *params);
Cons *new_Cons(t_point a, t_point b);
#define new_List(a)  new_Cons(a, NIL)
#define new_Param(a) new_Thunk(NIL, (Cons *) make_Num(a))

#define pnew_Function(a,b) make_Func(new_Function((a),(b)))
#define pnew_Thunk(a,b) make_Thunk(new_Thunk((a),(b)))
#define pnew_Cons(a,b) make_Cons(new_Cons((a),(b)))
#define pnew_List(a)  make_Cons(new_List(a))
#define pnew_Param(a) make_Thunk(new_Param(a))


#define is_NIL(p) ((p) == NIL)

static inline int is_Param(t_point p)
{
	if (!type_match(p, THUNK) || (p == (t_point) 2)) return 0;
	Thunk *t = get_Thunk(p);

	return (t->function == NIL && type_match((t_point) t->params, NUMBER));
}

#define is_Func(s)  (type_match(s, FUNCTION) && s != BOOL_TRUE)
#define is_Thunk(s) (type_match(s, THUNK) && !is_Param(s) && ((s) != (t_point) 2))
#define is_Bool(s)  ((s) == BOOL_TRUE || (s) == BOOL_FALSE)
#define is_Num(s)   type_match(s, NUMBER)
#define is_Cons(s)  type_match(s, CONS)

#endif
