#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mytype.h>
#include <tokens.h>

#ifndef PARSER_H
#define PARSER_H

// contadores de recursão
int E_lvl, T_lvl, F_lvl, A_lvl;				

// pilha de operandos (constantes numéricas apenas)	
double operand[MAX_STACK_SIZE]; 								
int sp;

// matriz de operadores (pilha de pilhas)
// pilha de nível de recursão com subpilhas de operadores
int oper[MAX_RECURSION_SIZE][MAX_STACK_SIZE];
int opsp;

// flag de permissão para execução de operações
int can_oper;														

// tabela de símbolos, armazenamento dos "lexemas" das variávies
char SYMTAB[MAX_SYMTAB_ENTRIES][MAX_ID_SIZE]; 		
// pilha de valores referente a tabela de símbolos
double acc[MAX_SYMTAB_ENTRIES]; 				
int nextentry;

int attr[MAX_STACK_SIZE];
int asp;

#endif // PARSER_H

void exec_attr(void);

/*
 * LL(1) expression grammar
 *
 * EBNF:
 *
 * expr -> [-] term { [+|-] term }
 *
 *
 *
 *                    --(+|-)---
 *        --(-)--     |        |
 *        |     |     |        |
 *        |     v     v        |
 * (expr)---------->(term)--->(?)-->(@)
 *
 */
double expr(void);
/*
 * term -> fact { [*|/] fact }
 *
 *             -------(*|/)-------
 *             |                 |
 *             v                 |
 * (term)--->(init)--->(fact)---(?)-->(end)
 *
 */
void term(void);
/*
 * fact -> ID
 *       | NUM
 *       | ( expr )
 *
 */
void fact(void);

extern void match(token_t);

extern void unmatch(token_t, const char*);

extern FILE *object;

extern void debug(const char *, ...);

extern void debug_oper(int [MAX_RECURSION_SIZE][MAX_STACK_SIZE], int , int );

extern token_t lookahead;

extern char lexeme[];