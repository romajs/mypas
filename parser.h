#include <stdio.h>

#include <lexer.h>
#include <symtab.h>

extern char id_list[MAX_SYMTAB_ENTRIES][MAX_ID_SIZE + 1];
extern int id_count;

void mypas(void);
/******************************************************************************
 **** Object declaration scope is defined next: *******************************
 *****************************************************************************/
/*
 * specification ->  vardeclr | sbrdeclr
 */
void specification(void);
/*
 * vardeclr ->  VAR idlist ':' typespec ';'
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
int typespec(void);
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
 * argspc -> [ VAR ] idlist ':' smptype
 */
void arspc(void);
/******************************************************************************
 **** Imperative scope is defined hereafter:
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
 */
void expr(void);
/*
 * whlstmt -> WHILE expr DO stmt
 */
void whlstmt(void);
/*
 * forstmt -> FOR ID '[' expr ']' ':=' expr DOWNTO|TO expr DO stmt
 */
void forstmt(void);
/*
 * repstmt -> REPEAT stmtlst UNTIL expr
 */
void repstmt(void);
/*
 * idstmt -> ID [ parm | { '[' expr ']' } ':=' expr ]
 */
void idstmt(void);
/*
 * parm -> '(' exprlst ')'
 */
void param(void);
/*
 * exprlst -> expr { ',' expr }
 */
void exprlst(void);

/*
 * EXTERN DECLARATIONS
 */
extern FILE *source, destination;

extern void match(token_t);

int symtab_add_list(int, char const [MAX_SYMTAB_ENTRIES][MAX_ID_SIZE + 1], int, int);
