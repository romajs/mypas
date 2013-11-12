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
q0:
	if(lookahead != BEGIN) {
		specification();
		goto q0;
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
	if(lookahead == VAR ) {
		vardeclr();
	} else {
		sbrdeclr();
	}	
}
/*
 * vardeclr ->  VAR idlist ':' typespec ';' { idlist ':' typespec ';' }
 */
void vardeclr(void)
{
  debug("vardeclr\n");
	/* semantic action */int type;/**/
	match(VAR);
q0:
	if(lookahead == ID) {
		idlist(); /* -> variable-name list*/
		match(':');
		type = typespec();/* ->simple-type attribute*/
		match(';');
		/* semantic rule: register the variable list with the same type
		 * in the symbol table
		 */symtab_add_list(id_count, id_list, type, scope);/**/
		goto q0;
	}	
}
/*
 * sbrdeclr ->  sbrhead { vardeclr } stmblk ';'
 */
void sbrdeclr(void)	
{
  debug("sbrdeclr\n");
	sbrhead();
	/* semantic action */ scope = LOCAL;
	int symtab_first_entry = symtab_next_entry;/**/
q0:
	if(lookahead != BEGIN) {
		vardeclr();
		goto q0;
	}
	stmblk();
	match(';');
	/* semantic action */ scope = GLOBAL;
	symtab_next_entry = symtab_first_entry;/**/
}
/*
 * sbrhead -> PROCEDURE ID argdef ';' | FUNCTION ID argdef ':' smptype ';'
 */
void sbrhead(void)
{
	/* semantic action */int type;/**/
	/* semantic action */ id_count = 0; /**/
  debug("sbrhead\n");
	if(lookahead == PROCEDURE) {
		match(PROCEDURE);
		/* semantic action */strcpy(id_list[id_count++], lexeme);
		match(ID);
		argdef();
		type = 0;
	} else {
		match(FUNCTION);
		/* semantic action */strcpy(id_list[id_count++], lexeme);
		match(ID);
		argdef();
		match(':');
		type = smptype(); /* ->simple-type attribute*/
	}
	match(';');
	/* semantic rule: register the variable list with the same type
	 * in the symbol table
	 */symtab_add_list(id_count, id_list, type, scope);/**/
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
  debug("argspc\n");
  /* semantic action */int type;/**/
	if(lookahead == VAR)
		match(VAR);
	idlist();
	match(':');
	type = smptype();
	/* semantic rule: register the variable list with the same type
	 * in the symbol table
	 */symtab_add_list(id_count, id_list, type, scope);/**/
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
q0:
	/* semantic action */ strcpy(id_list[id_count++], lexeme); /**/
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
  debug("typespec\n");
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
      // ERROR TYPE
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
  /* semantic action */Operand condition;/**/
	match(IF);
	 /* semantic action */condition = expr();
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
  /* semantic action */Operand condition;/**/
	match(WHILE);
	/* semantic action */condition = expr();
	match(DO);
	stmt();
}
/*
 * forstmt -> FOR ID [ '[' expr ']' ] ':=' expr DOWNTO|TO expr DO stmt
 */
void forstmt(void)
{
  debug("forstmt\n");
  /* semantic action */Operand attribution, dimension, condition;/**/
	match(FOR);
	match(ID);
	if(lookahead == '[') {
		match('[');
		/* semantic action */dimension = expr();
		match(']');
	}
	match(ATTR); // ':='
	/* semantic action */ attribution = expr();
	if(lookahead == TO) {
		match(TO);
	} else {
		match(DOWNTO);
	}
	/* semantic action */ condition = expr();
	match(DO);
	stmt();
}
/*
 * repstmt -> REPEAT stmtlst UNTIL expr
 */
void repstmt(void)
{
  debug("repstmt\n");
  /* semantic action */Operand condition;/**/
	match(REPEAT);
	stmtlst();
	match(UNTIL);
	/* semantic action */condition = expr();
} 
/*
 * idstmt -> ID [ param | [ '[' expr ']' ] [ ':=' expr ]
 */
void idstmt(void)
{
  debug("idstmt\n");
  /* semantic action */Operand attribution, dimension;/**/
	match(ID);
	if(lookahead == '(') {
		param();
	} else {
		if(lookahead == '[') {
			match('[');
			/* semantic action */dimension = expr();
			match(']');
		}
		match(ATTR); // ATTR = ':='
		/* semantic action */ attribution = expr();
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
  /* semantic action */ Operand parameter;/**/
q0:
  /* semantic action */ id_count = 0; /**/
	/* semantic action */ parameter = expr();
	/* semantic action */ strcpy(id_list[id_count++], parameter.lexeme);
	/* semantic rule: register the variable list with the same type
	 * in the symbol table
	 */symtab_add_list(id_count, id_list, parameter.type, scope);/**/
	if(lookahead == ',') {
		match(',');
		goto q0;
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