#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <keyword.h>
#include <mytype.h>
#include <tokens.h>

extern FILE* source;

extern FILE* object;

extern token_t lookahead;

extern char lexeme[];

extern keyword iskeyword(char const *);

extern void err(int, int, const char *, ...);