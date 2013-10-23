#include <stdio.h>

#include <parser.h>

extern void mypas(void);

extern token_t lookahead;

extern char lexeme[];

extern void debug(const char *, ...);