#ifndef ERROR_H
#define ERROR_H

#ifdef DEBUG
	#define __MAKE_ERROR(n) throw_full_error(n, __LINE__, __FILE__);
#else
	#define __MAKE_ERROR(n) throw_error(n);
#endif

#define ERROR(n)  {__MAKE_ERROR(n); return 0; }
#define ERROR_RET(n) (throw_full_error(n, __LINE__, __FILE__) ? NULL : NULL)


typedef enum {
	TYPE_ERROR,
	TOO_DEEP_RECURSION,
	TOO_MANY_PARAMS,
	INNER_ERROR,
	NOT_IMPLEMENTED,
	UNDEFINED,
	SYNTAX_ERROR,
} E_ERROR;

int throw_error(E_ERROR t);
int throw_full_error(E_ERROR t, int line, char file[]);

#endif
