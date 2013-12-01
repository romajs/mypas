#include <string.h>

#include <header/tokens.h>
#include <header/keyword.h>

#ifndef TYPECHECK_H
#define TYPECHECK_H

/* estrutura de operandos*/
struct _operand_ {
  double value;
  char *lexeme;
  int type;
};

typedef struct _operand_ Operand;

/* pilha de operandos (constantes numéricas apenas) */
Operand operand[MAX_STACK_SIZE];
int sp;

/* estrutura de operadores */
struct _oper_ {
	token_t symbol; /* símbolo */
	int level; /* nível de recursão (em E_lvl) */
};

typedef struct _oper_ Oper;

/* pilha de operadores */
Oper oper[MAX_STACK_SIZE];
int opsp;

#endif /* TYPECHECK_H */

/* error inteface */

void err(int, int, const char *, ...);

/* lexer interface */

extern token_t lookahead;

/* parser interface */

extern int E_lvl;