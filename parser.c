#include <parser.h>
/*
 * mypas -> PROGRAM ID ';' { specification } stmblk '.'
 */
void mypas(void)
{
	match(PROGRAM);
	match(ID);
	match(';');
m0:
	if(lookahead == VAR || lookahead == PROCEDURE || lookahead == FUNCTION) {
		specification();
		goto m0;
	}
	stmblk();
	match('.');
}	
/******************************************************************************
 **** Object declaration scope is defined next: *******************************
 *****************************************************************************/
/*
 * specification ->  vardeclr | sbrdeclr
 */
void specification(void)
{
	if(lookahead == VAR ) {
		vardeclr();
	} else {
		sbrdeclr();
	}	
}
/*
 * vardeclr ->  VAR idlist ':' typespec ';'
 */
void vardeclr(void)
{
	match(VAR);
	idlist();
	match(':');
	typespec();
	match(';');
}
/*
 * sbrdeclr ->  sbrhead { vardeclr } stmblk ';'
 */
void sbrdeclr(void)
{
	sbrhead();
sb0:
	if(lookahead == VAR) {
		vardeclr();
		goto sb0;
	}
	stmblk();
	match(';');
}
/*
 * idlist -> ID { ',' ID }
 */
void idlist(void)
{
il0:
	match(ID);
	if(lookahead == ',') {
		match(',');	
		goto il0;
	}
}	
/*
 * typespec -> smptype | ARRAY '[' UINT ']' OF typespec
 */
void typespec(void)
{
	if(lookahead == ARRAY) {
		match(ARRAY);
		match('[');
		match(UINT); // ???
		match(']');
		match(OF);
		typespec();
	}
	else {
		smptype();
	}
}
/*
 * smptype -> INTEGER | REAL | DOUBLE | BOOLEAN | STRING
 */
void smptype(void)
{
	switch(lookahead) {
		case INTEGER:
		case REAL:
		case DOUBLE:
		case BOOLEAN:
		case STRING:
			match(lookahead);
		default:
			;
	}
}
/*
 * sbrhead -> PROCEDURE ID argdef ';' | FUNCTION ID argdef ':' smptype ';'
 */
void sbrhead(void)
{
	if(lookahead == PROCEDURE) {
		match(PROCEDURE);
		match(ID);
		argdef();
	} else {
		match(FUNCTION);
		match(ID);
		argdef();
		match(':');
		smptype();
	}
	match(';');
}
/*
 * argdef -> [ '(' arglist ')' ]
 */
void argdef(void)
{
	if(lookahead == '(') {
		match('(');
		arglist();
		match(')');
	}
}
/*
 * arglist -> argspc { ';' argspc }
 */
void arglist(void)
{
al0:
	argspc();
	if(lookahead == ';') {
		match(';');
		goto al0;
	}
}
/*
 * argspc -> [ VAR ] idlist ':' smptype
 */
void arspc(void)
{
	if(lookahead == VAR)
		match(VAR);
	idlist();
	match(':');
	smptype();
}
/******************************************************************************
 **** Imperative scope is defined hereafter:
 *****************************************************************************/
/*
 * stmt -> <epsilon> | ifstmt | whlstmt | forstmt | repstmt | idstmt | stmblk
 */
void stmt(void)
{
	switch(lookahead){
		case BEGIN:
			stmtblk();
			break;
		case IF:
			ifstmt();
			break;
		case WHILE:
			whlstmt();
			break;
		case FOR:
			forstmt();
			break;
		case REPEAT:
			repstmt();
			break;
		case ID:
			idstmt();
			break;
		default:
			;
	}
}
/*
 * stmblk -> BEGIN stmtlst END
 */
void stmblk(void)
{
	match(BEGIN);
	stmtlst();
	match(END);
}
/*
 * stmtlst -> stmt { ';' stmt }
 */
void stmtlst(void)
{
s0:
	stmt();
	if(lookahead == ';') {
		match(';');
		goto s0;
	}
}
/* ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 *
 */
void ifstmt(void)
{
	match(IF);
	expr();
	match(THEN); 
	stmt();
	if(lookahead == ELSE){
		match(ELSE);
		stmt();
	}
}
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
/*
 * whlstmt -> WHILE expr DO stmt
 */
void whlstmt(void)
{
	match(WHILE);
	expr();
	match(DO);
	stmt();
}
/*
 * forstmt -> FOR ID '[' expr ']' ':=' expr DOWNTO|TO expr DO stmt
 */
void forstmt(void)
{
	match(FOR);
	match(ID);
	// ??? DAFUQ ???
	if(lookahead == TO) {
		match(TO);
	} else {
		match(DOWNTO);
	}
	expr();
	match(DO);
}
/*
 * repstmt -> REPEAT stmtlst UNTIL expr
 */
void repstmt(void)
{
	match(REPEAT);
	stmtlst();
	match(UNTIL);
	expr();
} 
/*
 * idstmt -> ID [ parm | { '[' expr ']' } ':=' expr ]
 */
void idstmt(void)
{
	match(ID);
	if(lookahead == '(') {
		param();
	} else {
i0:
		if(lookahead == '[') {
			match('[');
			expr();
			match(']');
			goto i0;
		}
		match(ATTR); // ATTR = ':='
		expr();
	}
}
/*
 * parm -> '(' exprlst ')'
 */
void param(void)
{
	match('(');
	exprlst();
	match(')');
}
/*
 * exprlst -> expr { ',' expr }
 */
void exprlst(void)
{
e0:
	expr();
	if(lookahead == ',') {
		match(',');
		goto e0;
	}
}
