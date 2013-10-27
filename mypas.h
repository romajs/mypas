#include <stdio.h>
#include <stdarg.h>

#include <args.h>
#include <parser.h>

extern void mypas(void);

extern token_t lookahead;

extern char lexeme[];

extern void debug(const char *, ...);

extern void match_args(int, char *[]);

void err(int, int, const char *, ...);