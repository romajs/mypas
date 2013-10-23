#include <stdarg.h>
#include <stdio.h>

#include <tokens.h>

void set_debug(FILE *);

void debug(const char *, ...);

void debug_symtab(const char [MAX_SYM_TAB][MAX_ID_LEN], const double *, int);

void debug_oper(int [MAX_RECURSION_SIZE][MAX_STACK_SIZE], int , int );