#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <tokens.h>

/* main interface */

extern FILE *source;

extern FILE *object;

/* error interface */

extern void err(int, int, const char *, ...);

/* lexer interface */

extern int lineno;