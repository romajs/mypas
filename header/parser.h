#include <stdio.h>

#include <header/lexer.h>
#include <header/symtab.h>

extern char id_list[MAX_SYMTAB_ENTRIES][MAX_ID_SIZE + 1];
extern int id_count;

/*
 * mypas -> PROGRAM ID ';' { specification } stmblk '.'
 */
void mypas(void);
/******************************************************************************
**** object declaration scope is defined next: ********************************
******************************************************************************/
/*
 * specification ->  vardeclr | sbrdeclr
 */
void specification(void);
/*
 * vardeclr ->  VAR idlist ':' typespec ';' { idlist ':' typespec ';' }
 */
void vardeclr(void);
/*
 * sbrdeclr ->  sbrhead { vardeclr } stmblk ';'
 */
void sbrdeclr(void);
/*
 * idlist -> ID { ',' ID }
 */
void idlist(void);
/*
 * typespec -> smptype | ARRAY '[' UINT ']' OF typespec
 */
void typespec(SEMANTIC_ATTRIB *);
/*
 * smptype -> INTEGER | REAL | DOUBLE | BOOLEAN | STRING
 */
int smptype(void);
/*
 * sbrhead -> PROCEDURE ID argdef ';' | FUNCTION ID argdef ':' smptype ';'
 */
void sbrhead(void);
/*
 * argdef -> [ '(' arglist ')' ]
 */
void argdef(void);
/*
 * arglist -> argspc { ';' argspc }
 */
void arglist(void);
/*
 * argspc -> [ VAR ] idlist ':' smptype { idlist ':' smptype }
 */
void argspc(void);
/******************************************************************************
 *** Imperative scope is defined hereafter: ***********************************
 *****************************************************************************/
/*
 * stmt -> <epsilon> | ifstmt | whlstmt | forstmt | repstmt | idstmt | stmblk
 */
void stmt(void);
/*
 * stmblk -> BEGIN stmtlst END
 */
void stmblk(void);
/*
 * stmtlst -> stmt { ';' stmt }
 */
void stmtlst(void);
/* ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 *
 */
void ifstmt(void);
/*
 * whlstmt -> WHILE expr DO stmt
 */
void whlstmt(void);
/*
 * forstmt -> FOR ID indexing ':=' expr DOWNTO|TO expr DO stmt
 */
void forstmt(void);
/*
 * repstmt -> REPEAT stmtlst UNTIL expr
 */
void repstmt(void);
/*
 * idstmt -> ID [ parm | indexing ':=' expr ]
 */
void idstmt(void);
/*
 * indexing -> {  '[' expr ']' }
 */
int indexing(int);
/*
 * parm -> '(' [ ')' | exprlst ')']
 */
int param(int);
/*
 * exprlst -> expr { ',' expr }
 */
int exprlst(int);

/******************************************************************************
*** Algebric and Boolean Expressions are defined hereafter: *******************
******************************************************************************/
int E_lvl, R_lvl, T_lvl, F_lvl;
/*
* expr -> E [ relop E ]
*
* relop -> '>' | '>=' | '<' | '<=' | '=' | '<>'
*
* E -> [ '-' | NOT ] term { addop term }
*
* addop -> '+' | '-' | OR
*
* term -> fact { mulop fact }
*
* mulop -> '*' | '/' | DIV | MOD | AND
*
* fact -> ID [ param | indexing ]
*/
void expr(void);
/*
 * mulop -> '*' | '/' | DIV | MOD | AND
 */
int ismulop(const token_t);
/*
 * addop -> '+' | '-' | OR
 */
int isaddop(const token_t);
/*
 * relop -> '>' | '>=' | '<' | '<=' | '=' | '<>'
 */
int isrelop(const token_t);
/******************************************************************************
 *** EXTERN DECLARATIONS ******************************************************
 *****************************************************************************/
 
/* main interface */
 
extern FILE *source;

extern FILE *object;

/* error interface */

extern void err(int, int, const char *, ...);

/* parser/lexer interface */

extern token_t lookahead;

extern char lexeme[];

extern void match(token_t);

/* symtab interface */

extern int symtab_entry;

int symtab_reset_entries(int);

int symtab_next_entry();

int symtab_lookup(const char *, int);

int symtab_retrieve(const char *, int);

int symtab_validate(const char*);

int symtab_add_list(int, char [MAX_SYMTAB_ENTRIES][MAX_ID_SIZE + 1], int, int, int, int[MAX_IND_SIZE]);

void set_subroutine_argument_list(int, int, int);