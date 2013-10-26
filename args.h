#include <stdio.h>

int D_FLAG; // flag de debug
int T_FLAG; // flag de testes

extern FILE *object;

extern void set_debug(FILE *);

void match_args(int, char *[]);