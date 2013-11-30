#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <header/tokens.h>
#include <header/keyword.h>

/* main interface */

extern FILE* source;

extern FILE* object;

/* error interface */

extern void err(int, int, const char *, ...);

/* keyword interface */

extern keyword iskeyword(char const *);

/* lexer interface */

extern token_t lookahead;

extern char lexeme[];

extern int lineno;

extern void skipspaces(FILE *);

extern token_t isEOF(FILE *);

extern token_t isID(FILE *);

extern token_t isNUM_CTE(FILE *);

extern token_t isSTR_CTE(FILE *);

extern token_t isATTR(FILE *);

extern token_t isRELOP(FILE *);

extern token_t gettoken(FILE *);

extern void match(token_t);