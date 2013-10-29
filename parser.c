#include <parser.h>

#define 	GLOBAL	0
#define	LOCAL		1
/* semantic variable*/int scope;/**/

/*
 * mypas -> PROGRAM ID ';' { specification } stmblk '.'
 */
void mypas(void)
{
  debug("mypas\n");
	// clear everything before start
	lookahead = EOF;
	lexeme[0] = 0;

  // call gramar initial symbol
	lookahead = gettoken(source);

	match(PROGRAM);
	match(ID);
	match(';');
	/* semantic action */ scope = GLOBAL;/**/
m0:
	if(lookahead != BEGIN) {
		specification();
		goto m0;
	}
	stmblk();
	match('.');
}	
/******************************************************************************
 **** object declaration scope is defined next: *******************************
 *****************************************************************************/
/*
 * specification -> vardeclr | sbrdeclr
 */
void specification(void)
{	
  debug("specification\n");
	/* semantic action */ scope = LOCAL;
	//int symtab_first_entry = symtab_next_entry;/**/
	if(lookahead == VAR ) {
		vardeclr();
	} else {
		sbrdeclr();
	}	
	/* semantic action */ scope = GLOBAL;
	//symtab_next_entry = symtab_first_entry;/**/
}
/*
 * vardeclr ->  VAR idlist ':' typespec ';'
 */
void vardeclr(void)
{
  debug("vardeclr\n");
	/* semantic action */int type;/**/
	match(VAR);
	idlist(); /* -> variable-name list*/
	match(':');
	type = typespec();/* ->simple-type attribute*/
	match(';');
	/* semantic rule: register the variable list with the ame type
	 * in the symbol table
	 *///symtab_add_list(id_count, id_list, type, scope);/**/
}
/*
 * sbrdeclr ->  sbrhead { vardeclr } stmblk ';'
 */
void sbrdeclr(void)	
{
  debug("sbrdeclr\n");
	sbrhead();
sb0:
	if(lookahead != BEGIN) {
		vardeclr();
		goto sb0;
	}
	stmblk();
	match(';');
}
/*
 * sbrhead -> PROCEDURE ID argdef ';' | FUNCTION ID argdef ':' smptype ';'
 */
void sbrhead(void)
{
  debug("sbrhead\n");
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
  debug("argdef\n");
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
  debug("arglist\n");
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
void argspc(void)
{
  debug("argspc\n");
	if(lookahead == VAR)
		match(VAR);
	idlist();
	match(':');
	smptype();
}
/*
 * idlist -> ID { ',' ID }
 */
char id_list[MAX_SYMTAB_ENTRIES][MAX_ID_SIZE + 1];
int id_count = 0;
void idlist(void)
{
  debug("idlist\n");
	/* semantic action */ id_count = 0; /**/
il0:
	/* semantic action */ strcpy(id_list[id_count++], lexeme); /**/
	match(ID);
	if(lookahead == ',') {
		match(',');	
		goto il0;
	}
}	
/*
 * typespec -> smptype | ARRAY '[' UINT ']' OF typespec
 */
typespec(void)
{
  debug("typespec\n");
ts0:
	if(lookahead == ARRAY) {
		match(ARRAY);
		match('[');
		match(UINT); // ???
		match(']');
		match(OF);
		goto ts0;
	}
	else {
		return smptype();
	}
}
/*
 * smptype -> INTEGER | REAL | DOUBLE | BOOLEAN | STRING
 */
int smptype(void)
{
  debug("smptype\n");
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
			return -1;
	}
}
/******************************************************************************
 *** Imperative scope is defined hereafter: ***********************************
 *****************************************************************************/
/*
 * stmblk -> BEGIN stmtlst END
 */
void stmblk(void)
{
  debug("stmblk\n");
	match(BEGIN);
	stmtlst();
	match(END);
}
/*
 * stmtlst -> stmt { ';' stmt }
 */
void stmtlst(void)
{
  debug("stmlst\n");
s0:
	stmt();
	if(lookahead == ';') {
		match(';');
		goto s0;
	}
}
/*
 * stmt -> <epsilon> | ifstmt | whlstmt | forstmt | repstmt | idstmt | stmblk
 */
void stmt(void)
{
  debug("stmt\n");
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
  debug("ifstmt\n");
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
  debug("whlstmt\n");
	match(WHILE);
	expr();
	match(DO);
	stmt();
}
/*
 * forstmt -> FOR ID [ '[' expr ']' ] ':=' expr DOWNTO|TO expr DO stmt
 */
void forstmt(void)
{
  debug("forstmt\n");
	match(FOR);
	match(ID);
	if(lookahead == '[') {
		match('[');
		expr();
		match(']');
	}
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
  debug("repstmt\n");
	match(REPEAT);
	stmtlst();
	match(UNTIL);
	expr();
} 
/*
 * idstmt -> ID [ param | [ '[' expr ']' ] [ ':=' expr ]
 */
void idstmt(void)
{
  debug("idstmt\n");
	match(ID);
	if(lookahead == '(') {
		param();
	} else {
		if(lookahead == '[') {
			match('[');
			expr();
			match(']');
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
  debug("param\n");
	match('(');
	exprlst();
	match(')');
}
/*
 * exprlst -> expr { ',' expr }
 */
void exprlst(void)
{
  debug("exprlst\n");
e0:
	expr();
	if(lookahead == ',') {
		match(',');
		goto e0;
	}
}
/******************************************************************************
 *** Algebric and Boolean Expressions are defined hereafter: ******************
 *****************************************************************************/
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
 * fact -> ID [ param | [ '[' expr ']' ] [ ':=' expr ]
 */
// Implementation in file 'mycalc.c'