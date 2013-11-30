#include <header/parser.h>

/*
 * mypas -> PROGRAM ID ';' { specification } stmblk '.'
 */
void mypas(void)
{
	match(PROGRAM);
	match(ID);
	match(';');
q0:
	if(lookahead != BEGIN) {
		specification();
		goto q0;
	}
	stmblk();
	match('.');
}	
/*****************************************************************************
*** object declaration scope is defined next: ********************************
*****************************************************************************/
/*
 * specification -> vardeclr | sbrdeclr
 */
void specification(void)
{		
	if(lookahead == VAR) {
		vardeclr();
	} else {
		sbrdeclr();
	}	
}
/*
 * vardeclr ->  VAR idlist ':' typespec ';' { idlist ':' typespec ';' }
 */
/** OBS: Esta parte foi modificada (está diferente do Eraldo) para aceitar mais 
	de uma declaração por VAR **/
void vardeclr(void)
{	
	match(VAR);
q0:
	if(lookahead == ID) {
		idlist();
		match(':');
		typespec();
		match(';');
		goto q0;
	}	
}
/*
 * sbrdeclr ->  sbrhead { vardeclr } stmblk ';'
 */
void sbrdeclr(void)	
{	
	sbrhead();
q0:
	if(lookahead != BEGIN) {
		vardeclr();
		goto q0;
	}
	stmblk();
	match(';');
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
q0:
	argspc();
	if(lookahead == ';') {
		match(';');
		goto q0;
	}
}
/*
 * argspc -> [ VAR ] idlist ':' smptype 
 */
void argspc(void)
{	
	if(lookahead == VAR)
	match(VAR);
	idlist();
	match(':');
	smptype();
}
/*
* idlist -> ID { ',' ID }
*/
void idlist(void)
{	
q0:
	match(ID);
	if(lookahead == ',') {
		match(',');	
		goto q0;
	}
}	
/*
 * typespec -> smptype | ARRAY '[' UINT ']' OF typespec
 */
typespec(void)
{	
q0:
	if(lookahead == ARRAY) {
		match(ARRAY);
		match('[');
		match(INT_CTE); // Our INT_CTE is already UNSIGNED!
		match(']');
		match(OF);
		goto q0;
	}
	else {
		smptype();
	}
}
/*
 * smptype -> INTEGER | REAL | DOUBLE | BOOLEAN | STRING
 */
int smptype(void)
{	
	switch(lookahead) {
	case INTEGER:
		match(INTEGER);
		return 1;
	case REAL:
		match(REAL);
		return 2;
	case DOUBLE:
		match(DOUBLE);
		return 3;
	case BOOLEAN:
		match(BOOLEAN);
		return 4;
	case STRING:
		match(STRING);
		return 5;
	default:
		// ERROR TYPE
		return -1;
	}
}
/*****************************************************************************
*** Imperative scope is defined hereafter: ***********************************
*****************************************************************************/
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
q0:
	stmt();
	if(lookahead == ';') {
		match(';');
		goto q0;
	}
}
/*
* stmt -> <epsilon> | ifstmt | whlstmt | forstmt | repstmt | idstmt | stmblk
*/
void stmt(void)
{	
	switch(lookahead){
	case BEGIN:
		stmblk();
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
 * ifstmt -> IF expr THEN stmt [ ELSE stmt ]
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
 * forstmt -> FOR ID indexing ':=' expr DOWNTO|TO expr DO stmt
 */
void forstmt(void)
{	
	match(FOR);
	match(ID);
	indexing();
	match(ATTR); // ':='
	expr();
	if(lookahead == TO) {
		match(TO);
	} else {
		match(DOWNTO);
	}
	expr();
	match(DO);
	stmt();
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
 * idstmt -> ID [ param | indexing ':=' expr ]
 */
 /**OBS: indexing é opcional**/
void idstmt(void)
{	
	match(ID);
	if(lookahead == '(') {
		param();
	} else {
		indexing();
		if(lookahead == ATTR) {
			match(ATTR); // ATTR = ':='
			expr();
		}
	}
}
/*
 * indexing -> {  '[' expr ']' }
 */
void indexing(void)
{	
q0:
  if(lookahead == '[') {
	 match('[');
	 expr();
	 match(']');
	 goto q0;
  }
}
/*
 * parm -> '(' [ ')' | exprlst ')']
 */
/**OBS: exprlst é opcional**/
void param(void)
{	
	if(lookahead == '(') {
		match('(');
		if(lookahead == ')') {
			match(')');
		} else {
			exprlst();
			match(')');
		}
	}
}
/*
 * exprlst -> expr { ',' expr }
 */
void exprlst(void)
{	
q0:
	expr();
	if(lookahead == ',') {
		match(',');
		goto q0;
	}
}
/******************************************************************************
*** Algebric and Boolean Expressions are defined hereafter: *******************
******************************************************************************/
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
void expr(void)
{ 	
	E_lvl = -1, R_lvl = -1, T_lvl = -1, F_lvl = -1;

	E: E_lvl++;

	switch(lookahead) { // inversão de sinal ('-') e negação (NOT)
    case '-':
    case NOT:      
      match(lookahead);
      break;
	}

	T: T_lvl++;
  
	R: R_lvl++;

	F: F_lvl++;

	switch(lookahead) {
    case ID:
      match(ID);
		if(lookahead == '(') {
			param();
		} 
		/**OBS: indexing é opcional**/
		indexing();
      break;
    /**************************************************************************
    *** Constants defs goes next: INTEGER | REAL | DOUBLE | BOOLEAN | STRING **
    **************************************************************************/
    case TRUE: 
    case FALSE:
    case INT_CTE:
    case REAL_CTE:
    case DBL_CTE:
    case STR_CTE:
      match(lookahead);
      break;
    /**************************************************************************
     *** End of Constants defitions *******************************************
     *************************************************************************/
    case '(':
      match('(');
      goto E;
    default:
      /* se não for nenhum dos esperados, então não faz parte da gramática,
       * sai com erro para não ficar tavado em 'loop' */
      err(FATAL, LEXICAL, "Token mismatch \"%s\"\n", lexeme);
	}	
 
	_F: F_lvl--;
  
	if(isrelop(lookahead)) {
		match(lookahead);
		goto R;
	}
 
	_R: R_lvl--;

	if(ismulop(lookahead)) {
		match(lookahead);
		goto F;
	}

	_T: T_lvl--;

	if(isaddop(lookahead)) {
		match(lookahead);
		goto T;
	}

	_E: E_lvl--;

	if(E_lvl > -1) {
		match(')');
		goto _F;
	}
}
/*
 * mulop -> '*' | '/' | DIV | MOD | AND
 */
ismulop(const token_t token)
{	
	switch(token) {
		case '*':
		case '/':
		case DIV:
		case MOD:
		case AND:
			return 1;
		default:
			return 0;
	}
}

/*
 * addop -> '+' | '-' | OR
 */
isaddop(const token_t token)
{	
	switch(token) {
		case '+':
		case '-':
		case OR:
			return 1;
		default:
			return 0;
	}
}
/*
 * relop -> '>' | '>=' | '<' | '<=' | '=' | '<>'
 */
isrelop(const token_t token)
{	
	switch(token) {
		case EQ:
		case NEQ:
		case GRT:
		case GEQ:
		case LSR:
		case LEQ:
			return 1;
		default:
			return 0;
	}
}