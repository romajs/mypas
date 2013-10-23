#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <keyword.h>
#include <mytype.h>
#include <tokens.h>

extern FILE* source, destination;

extern token_t lookahead;

extern char lexeme[];

extern int iskeyword(char const *);