#include <stdio.h>
#include <glob.h>

extern FILE *object;

extern FILE *source;

extern int error;

int test(const char*);

void tests(void);

extern int symtab_lookup(const char *);