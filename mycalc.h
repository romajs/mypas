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

// pilha de operandos (constantes numéricas apenas)	
double operand[MAX_STACK_SIZE]; 								
int sp;

// matriz de operadores (pilha de pilhas)
// pilha de nível de recursão com subpilhas de operadores
int oper[MAX_RECURSION_SIZE][MAX_STACK_SIZE];
int opsp;

// flag de permissão para execução de operações
int can_oper;														

#endif // PARSER_H

/*
 * LL(1) expression grammar
 *
 * EBNF:
 *
 *
 * expr -> {attr} [-] term { [+|-] term }
 *
 *                         --(+|-)--
 *            -(-)-        |       |
 *            |   |        |       |
 *            |   v        v       |
 * (E)------------------->(T)--------->(_E)
 *
 * 
 * term -> fact { [*|/] fact }
 *
 *             --(*|/)--
 *             |       |
 *             v       |
 * (T)------->(F)--------->(_T)
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

// mulop -> '*' | '/' | DIV | MOD | AND 
int ismulop(const token_t);

// addop -> '+' | '-' | OR
int isaddop(const token_t);

// relop -> '>' | '>=' | '<' | '<=' | '=' | '<>'
int isrelop(const token_t);
 
double expr(void);

extern token_t lookahead;

extern char lexeme[];

extern void match(token_t);

extern void unmatch(token_t, const char*);

extern FILE *object;

extern void debug(const char *, ...);

extern void debug_oper(int [MAX_RECURSION_SIZE][MAX_STACK_SIZE], int , int );