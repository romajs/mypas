#include <header/error.h>
#include <header/parser.h>

/* main interface */

FILE *source;

FILE *object;

extern void mypas(void);

/* error interface */

extern void err(int, int, const char *, ...);

/* lexer interface */

extern token_t lookahead;