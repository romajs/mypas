#include <parser.h>

#define 	GLOBAL	0
#define	LOCAL		1

/*sa*/int scope;/**/

/*
 * mypas -> PROGRAM ID ';' { specification } stmblk '.'
 */
void mypas(void)
{
	debug("<mypas>\n");
	// clear everything before start
	lookahead = EOF;
	lexeme[0] = 0;

	// call gramar initial symbol
	lookahead = gettoken(source);

	match(PROGRAM);
	match(ID);
	match(';');
	/*sa*/scope = GLOBAL;/**/
q0:
	debug("mypas:q0\n");
	if(lookahead != BEGIN) {
		specification();
		goto q0;
	}
	stmblk();
	match('.');
	debug("</mypas>\n");
}	
/*****************************************************************************
*** object declaration scope is defined next: ********************************
*****************************************************************************/
/*
 * specification -> vardeclr | sbrdeclr
 */
void specification(void)
{	
	debug("<specification>\n");
	if(lookahead == VAR) {
		vardeclr();
	} else {
		sbrdeclr();
	}	
	debug("</specification>\n");
}
/*
 * vardeclr ->  VAR idlist ':' typespec ';' { idlist ':' typespec ';' }
 */
/** OBS: Esta parte foi modificada (está diferente do Eraldo) para aceitar mais 
	de uma declaração por VAR **/
void vardeclr(void)
{
	debug("<vardeclr>\n");
	/*sa*/SEMANTIC_ATTRIB *satrb;/**/
	match(VAR);
q0:
	/*sa*/satrb = malloc(sizeof(SEMANTIC_ATTRIB));/**/
	/*sa*/satrb->scope = scope;/**/
	if(lookahead == ID) {
		idlist();
		match(':');
		/*sa*/typespec(satrb);/**/
		match(';');
		/*sa*/symtab_add_list(id_count, id_list, satrb->type, satrb->scope, satrb->indirections, satrb->dimension);/**/
		goto q0;
	}	
	debug("</vardeclr>\n");
}
/*
 * sbrdeclr ->  sbrhead { vardeclr } stmblk ';'
 */
void sbrdeclr(void)	
{
	debug("<sbrdeclr>\n");
	// troca o escopo para LOCAL para as declarações de argumentos e variaveis da subrotina
	/*sa*/scope = LOCAL;/**/	
	/**/int symtab_first_entry = symtab_next_entry;/**/
	/*sa*/SEMANTIC_ATTRIB *satrb = malloc(sizeof(SEMANTIC_ATTRIB));/**/
	/*sa*/sbrhead(satrb);/**/
	
q0:
	if(lookahead != BEGIN) {
		vardeclr();
		goto q0;
	}
	stmblk();
	match(';');
	
	// TODO: apagar todas as declarações locais de 'symlist'
	/**/symtab_next_entry = symtab_first_entry;/**/ 		
	
	/*sa*/scope = GLOBAL;/**/ // volta escopo ao atual
	
	// completa atributos de 'satrb'
	/*sa*/satrb->scope = scope;/**/	
	/*sa*/satrb->indirections = 0;/**/
	
	//  adiciona a subrotina a tabela de símbolos
	symtab_add(satrb);
	debug("</sbrdeclr>\n");
}
/*
 * sbrhead -> PROCEDURE ID argdef ';' | FUNCTION ID argdef ':' smptype ';'
 */
void sbrhead(SEMANTIC_ATTRIB *satrb)
{
	debug("sbrhead\n");
	/*sa*/int symtab_first_entry = symtab_next_entry;/**/
	
	if(lookahead == PROCEDURE) {
		match(PROCEDURE);
		/*sa*/strcpy(satrb->name, lexeme);/**/ // salva o lexema da subrotina
		match(ID);
		argdef();
		/*sa*/satrb->type = 0;/**/ // (SEM TIPO)
	} else {
		match(FUNCTION);
		/*sa*/strcpy(satrb->name, lexeme);/**/ // salva o lexema da subrotina
		match(ID);
		argdef();
		match(':');
		/*sa*/satrb->type = smptype();/**/
	}	
	// TODO:: copiar variaveis declaradas em 'argdef' para os parametros da subrotina
	symtab_asgm_params(symtab_first_entry, symtab_next_entry, satrb);
	/*satrb->param = malloc((symtab_next_entry - symtab_first_entry) *  sizeof(SEMANTIC_ATTRIB));
	int i;
	for(i = symtab_first_entry, satrb->attributes = 0; i < symtab_next_entry; i++, satrb->attributes++) {
		memcpy(&satrb->param[satrb->attributes], &symtab[i], sizeof(SEMANTIC_ATTRIB));
	}*/
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
	/*sa*/int type;/**/
	if(lookahead == VAR)
	match(VAR);
	idlist();
	match(':');
	type = smptype();
	/*sa*/symtab_add_list(id_count, id_list, type, scope, 0, malloc(sizeof(int)*MAX_IND_SIZE));/**/
}
/*
* idlist -> ID { ',' ID }
*/
char id_list[MAX_SYMTAB_ENTRIES][MAX_ID_SIZE + 1];
int id_count = 0;
void idlist(void)
{
	debug("idlist\n");
	/*sa*/id_count = 0;/**/
q0:
	/*sa*/strcpy(id_list[id_count++], lexeme);/**/
	match(ID);
	if(lookahead == ',') {
		match(',');	
		goto q0;
	}
}	
/*
 * typespec -> smptype | ARRAY '[' UINT ']' OF typespec
 */
void typespec(SEMANTIC_ATTRIB *satrb)
{
	debug("typespec\n");
	/*sa*/satrb->indirections = 0;/**/
	/*sa*/satrb->type = 0;/**/ // CUIDADO CONFUNDIR COM PROCEDURE!!!
q0:
	if(lookahead == ARRAY) {
		match(ARRAY);
		match('[');		
		/*sa*/satrb->dimension[satrb->indirections++] = atoi(lexeme);/**/
		match(INT_CTE); // Our INT_CTE is already UNSIGNED!
		match(']');
		match(OF);
		goto q0;
	}
	else {
		/*sa*/satrb->type = smptype();/**/
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
/*****************************************************************************
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
 * forstmt -> FOR ID indexing ':=' expr DOWNTO|TO expr DO stmt
 */
void forstmt(void)
{
	debug("forstmt\n");
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
	debug("repstmt\n");
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
	debug("idstmt\n");
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
	debug("indexing\n");
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
	debug("param\n");
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
	debug("exprlst\n");
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
	debug("expr\n");
	E_lvl = -1, R_lvl = -1, T_lvl = -1, F_lvl = -1;

	E: debug( "E: %d\n", ++E_lvl);

	switch(lookahead) { // inversão de sinal ('-') e negação (NOT)
    case '-':
    case NOT:
      debug( "(signal reversion) activated.\n");
      match(lookahead);
      break;
	}

	T: debug("T: %d\n", ++T_lvl);	
  
	R: debug("R: %d\n", ++R_lvl);

	F: debug("F: %d\n", ++F_lvl); 	

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
      // se não for nenhum dos esperados, então não faz parte da gramática
      err(FATAL, LEXICAL, "Token mismatch \"%s\"\n", lexeme);
	}	
 
	_F: debug( "_F: %d\n", --F_lvl);
  
	if(isrelop(lookahead)) {
		match(lookahead);
		goto R;
	}
 
	_R: debug( "_R: %d\n", --R_lvl);

	if(ismulop(lookahead)) {
		match(lookahead);
		goto F;
	}

	_T: debug( "_T: %d\n", --T_lvl);

	if(isaddop(lookahead)) {
		match(lookahead);
		goto T;
	}

	_E: debug( "_E: %d\n", --E_lvl);

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
	debug("ismulop\n");
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
	debug("isaddop\n");
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
	debug("isrelop\n");
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