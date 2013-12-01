#include <stdio.h>
#include <stdarg.h>

/* main interface */

extern FILE* object;

/* codegen interface */

void mainlbl();

void setlbl(int);

void gofalse(int);

void gotolbl(int);

void gotosteplbl(int, int);

void pop(int);

void mov(int);

void cmp(const char*);

void jg(int);

void jl(int);

void stepc(int, const char*);

void inc(const char*);

void dec(const char*);