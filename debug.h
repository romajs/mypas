#include <stdarg.h>
#include <stdio.h>

#include <tokens.h>

void set_debug(int);

void debug(const char *, ...);

void debug_symtab(const char [MAX_SYMTAB_ENTRIES][MAX_ID_SIZE], const double *, int);

void debug_oper(int [MAX_RECURSION_SIZE][MAX_STACK_SIZE], int , int );