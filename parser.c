/*
 * mypas -> PROGRAM ID ';'  { specification } stmblk '.'
 *
 ******************************************************************************
 **** Object declaration scope is defined next: *******************************
 ******************************************************************************
 *
 * specification ->  vardeclr | sbrdeclr
 *
 * vardeclr ->  VAR idlist ':' typespec ';'
 *
 * sbrdeclr ->  sbrhead { vardeclr } stmblk ';'
 *
 * idlist -> ID { ',' ID }
 *
 * typespec -> smptype | ARRAY '[' UINT ']' OF typespec
 *
 * smptype -> INTEGER | REAL | DOUBLE | BOOLEAN | STRING
 *
 * sbrhead -> PROCEDURE ID argdef ';'
 * 	    | FUNCTION ID argdef ':' smptype ';'
 *
 * argdef -> [ '(' arglist ')' ]
 *
 * arglist -> argspc { ';' argspc }
 *
 * argspc -> [ VAR ] idlist ':' smptype
 *
 ******************************************************************************
 **** Imperative scope is defined hereafter:
 ******************************************************************************
 *
 * stmt -> <epsilon> | ifstmt | whlstmt | forstmt | repstmt
 * 	 | idstmt | stmblk
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
 *
 * stmtlst -> stmt { ';' stmt }
 *
 * ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 *
 */
void ifstmt(void)
{
	match(IF); expr(); match(THEN); stmt();
	if(lookahead==ELSE){
		match(ELSE); stmt();
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
 *
 * whlstmt -> WHILE expr DO stmt
 *
 * forstmt -> FOR ID indexing ':=' expr [DOWN]TO expr DO stmt
 *
 * repstmt -> REPEAT stmtlst UNTIL expr
 * 
 * idstmt -> ID [ parm | { '[' expr ']' } ':=' expr ]
 *
 * parm -> '(' exprlst ')'
 *
 * exprlst -> expr { ',' expr }
 *
 *
 */
