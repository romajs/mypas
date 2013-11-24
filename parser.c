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
		/*sa*/symtab_add_list(id_count, id_list, satrb->type, satrb->scope,
      satrb->indirections, satrb->dimension);/**/
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
	/* store begin of subroutine declarations plus the declaration itself */
  int symtab_first_entry = symtab_next_entry + 1;
	sbrhead();	
q0:
	if(lookahead != BEGIN) {
		vardeclr();
		goto q0;
	}
	stmblk();
	match(';');
	
	// TODO: limpar os valores das propriedades do array de symtab
	/* reset symtab next entry, eliminating LOCAL declarations */
  symtab_next_entry = symtab_first_entry;
	
	/* volta escopo p/ global */scope = GLOBAL;
	
	debug("</sbrdeclr>\n");
}
/*
 * sbrhead -> PROCEDURE ID argdef ';' | FUNCTION ID argdef ':' smptype ';'
 */
void sbrhead()
{
	debug("sbrhead\n");  
	/* reserve one address to subroutine */int entry = symtab_next_entry++;
  /* fill subroutine attributes */
	/* set subroutine scope to GLOBAL */symtab[entry].scope = scope;
  /* set object type to SUBROUTINE */symtab[entry].objtype = SUBROUTINE;
	/* it will never have indirections */symtab[entry].indirections = 0;
  
	/* change current scope to LOCAL */scope = LOCAL;  
	/* store subroutine declarations */int symtab_first_entry;
  
  /* REMINDER: the subroutine itself needs to become declared (stored in symtab) 
    before 'stmblk' because it might have recursion calls in the subroutine. The
    ideal should separate 'sbrdhead' from 'sbrbody' for example, but lexer/parser
    is already defined */
	
	if(lookahead == PROCEDURE) {
		match(PROCEDURE);
    /* validate id subroutine */symtab_validate(lexeme);
		/* save subroutine lexeme */strcpy(symtab[entry].name, lexeme);
    debug("%s was added susessfully to symtab\n", symtab[entry].name);
    debug_symtab_entry(entry);
		match(ID);    
    /* store begin of parameter declarations */symtab_first_entry = symtab_next_entry;
		argdef();
		/* procedure has no type (0) */symtab[entry].type = 0;
	} else {
		match(FUNCTION);
    /* validate id subroutine */symtab_validate(lexeme);
		/* save subroutine lexeme */strcpy(symtab[entry].name, lexeme);      
    debug("%s was added susessfully to symtab\n", symtab[entry].name);
    debug_symtab_entry(entry);
    /* In case of FUNCTION, a variable with same name must be added to symtab
      to be used internally in 'stmblk' to set the returng value from the
      function, but the scope of this variavel need to become current scope */
    /* saves one position in symtab */int entry_var = symtab_next_entry++;
    symtab[entry_var] = symtab[entry];    
    symtab[entry_var].scope = scope;   
    symtab[entry_var].objtype = 0;
    debug("%s was added susessfully to symtab\n", symtab[entry].name);
    debug_symtab_entry(entry_var);
		match(ID);    
    /* store begin of parameter declarations */symtab_first_entry = symtab_next_entry;
		argdef();
		match(':');
		/* set subroutine type */symtab[entry].type = smptype();
    /* set var type as the same */symtab[entry_var].type = symtab[entry].type;
	}	
	/* assign all parameters declared in 'argdef' to subroutine param list */
	set_subroutine_param_list(symtab_first_entry, symtab_next_entry, entry);
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
  int entry = symtab_retrieve(lexeme, 0);
	match(ID);
	indexing(symtab[entry].indirections);
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
  int entry = symtab_retrieve(lexeme, 0);
	match(ID);
	if(lookahead == '(') {
		param(symtab[entry].attributes);
	} else {
		indexing(symtab[entry].indirections);
		if(lookahead == ATTR) {
			match(ATTR); // ATTR = ':='
			expr();
		}
	}
}
/*
 * indexing -> {  '[' expr ']' }
 */
indexing(int indexlevel)
{
	debug("<indexing>\n");
  /* REMINDER: indexlevel is decremented as '[' is matched, and it may not
    overload indirections entrie, but it can become lower */
q0:
  if(lookahead == '[') {
    indexlevel--;
    match('[');
    expr();
    match(']');
    goto q0;
  }  
  debug("\tindexlevel = %d\n", indexlevel);
  if(indexlevel < 0) {
    err(FATAL, SEMANTIC, "Too many index levels.\n");
  }
  debug("</indexing>\n");
  return indexlevel >= 0;
}
/*
 * parm -> '(' [ ')' | exprlst ')']
 */
/**OBS: exprlst é opcional**/
param(int paramindex)
{
	debug("<param>\n");
  /* REMINDER: paramindex must match attributes entries */
	if(lookahead == '(') {
		match('(');
		if(lookahead == ')') {
			match(')');
		} else {
			paramindex = exprlst(paramindex);
			match(')');
		}
	}
  /* REMINDER: paramindex must be treated here and not in 'exprlst', because it
    may have none */    
  debug("\tparamindex = %d\n", paramindex);
  if(paramindex < 0) {
    err(FATAL, SEMANTIC, "Too many arguments.\n");
  } else if(paramindex > 0) {
    err(FATAL, SEMANTIC, "Too few arguments.\n");
  }
  debug("</param>\n");
  return paramindex == 0;
}
/*
 * exprlst -> expr { ',' expr }
 */
int exprlst(int paramindex)
{
	debug("exprlst\n");
   /* REMINDER: indexlevel is decremented as '[' is matched, and it may not
    overload indirections entrie, but it can become lower */
q0:
	expr();
  paramindex--;
  debug("\tparamindex = %d\n", paramindex);
	if(lookahead == ',') {
		match(',');
		goto q0;
	}
  return paramindex;
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

  /* id symtab position */int entry;
	switch(lookahead) {
    case ID:
      /* search for id in symtab */entry = symtab_retrieve(lexeme, 1);
      debug_symtab_entry(entry);
      match(ID);      
      if(lookahead == '(') {
        /* match entry attributes */param(symtab[entry].attributes);
      } 
      /**OBS: indexing é opcional**/
      /* match entry indirections */indexing(symtab[entry].indirections);		
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