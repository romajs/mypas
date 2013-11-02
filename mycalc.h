#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mytype.h>
#include <tokens.h>
#include <keyword.h>

#ifndef PARSER_H
#define PARSER_H

// contadores de recursão
int E_lvl, R_lvl, T_lvl, F_lvl;				

// estrutura de operandos
struct _operand_ {
  double value;
  char *lexeme;
  int type;
};

typedef struct _operand_ Operand;

// pilha de operandos (constantes numéricas apenas)	
Operand operand[MAX_STACK_SIZE]; 								
int sp;

// estrutura de operadores
struct _oper_ {
	token_t symbol; // símbolo
	int level; // nível de recursão (em E_lvl)
};

typedef struct _oper_ Oper;

// pilha de operadores
Oper oper[MAX_STACK_SIZE];
int opsp;											

#endif // PARSER_H

/*
 * LL(1) expression grammar
 *
 * EBNF:
 *
 *
 * expr -> [-] term { [+|-] term }
 *
 *                         --(+|-)--
 *            -(-)-        |       |
 *            |   |        |       |
 *            |   v        v       |
 * (E)------------------->(T)--------->(_E)
 *
 * 
 * term -> relp { [*|/] relp }
 *
 *             --(*|/)--
 *             |       |
 *             v       |
 * (T)------->(R)--------->(_T)
 *
 *
 * relp -> fact { [*|/] fact }
 *
 *             --(=|<>|>|>=|<|<=)--
 *             |                  |
 *             v                  |
 * (R)------->(F)-------------------->(_R)
 * 
 * 
 * fact -> ID ['=' expr] | NUM | '('expr')'
 *
 * (F)---------------> (ID) --------------\
 *  | \                                    \
 *  |  \------------> (NUM) ------------->(_F)
 *  |                                      /
 *   \-----> '(' ----> (E) ----> ')' -----/
 * 
 */
double expr(void);

// mulop -> '*' | '/' | DIV | MOD | AND 
int ismulop(const token_t);

// addop -> '+' | '-' | OR
int isaddop(const token_t);

// relop -> '>' | '>=' | '<' | '<=' | '=' | '<>'
int isrelop(const token_t);

/*
 * Extern declarations
 */

extern token_t lookahead;

extern char lexeme[];

extern void match(token_t);

extern void unmatch(token_t, const char*);

extern FILE *object;

extern void debug(const char *, ...);

extern void debug_oper(int [MAX_RECURSION_SIZE][MAX_STACK_SIZE], int , int );