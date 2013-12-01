#include <header/parser.h>

#define 	GLOBAL	0
#define	LOCAL		1

/*sa*/int scope;/**/

/*
 * mypas -> PROGRAM ID ';' { specification } stmblk '.'
 */
void mypas(void)
{	
	match(PROGRAM);
	match(ID);
	match(';');
	/*sa*/scope = GLOBAL;/**/
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
}
/*
 * sbrdeclr ->  sbrhead { vardeclr } stmblk ';'
 */
void sbrdeclr(void)	
{	
	/* store begin of subroutine declarations plus the declaration itself */
  int symtab_first_entry = symtab_entry + 1;
	sbrhead();	
q0:
	if(lookahead != BEGIN) {
		vardeclr();
		goto q0;
	}
	stmblk();
	match(';');
	
	/* reset symtab entries, eliminating LOCAL declarations */
  symtab_reset_entries(symtab_first_entry);
	
	/* volta escopo p/ global */scope = GLOBAL;	
}
/*
 * sbrhead -> PROCEDURE ID argdef ';' | FUNCTION ID argdef ':' smptype ';'
 */
void sbrhead()
{	
	/* reserve one address to subroutine */int entry = symtab_next_entry();
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
    
		match(ID);    
    /* store begin of arg declarations */symtab_first_entry = symtab_entry;
		argdef();
		/* procedure has no type (0) */symtab[entry].type = 0;
	} else {
		match(FUNCTION);
    /* validate id subroutine */symtab_validate(lexeme);
		/* save subroutine lexeme */strcpy(symtab[entry].name, lexeme);      

    /* In case of FUNCTION, a variable with same name must be added to symtab
      to be used internally in 'stmblk' to set the returng value from the
      function, but the scope of this variavel need to become current scope */
    /* saves one position in symtab */int entry_var = symtab_next_entry();
    symtab[entry_var] = symtab[entry];    
    symtab[entry_var].scope = scope;   
    symtab[entry_var].objtype = 0;

		match(ID);    
    /* store begin of arg declarations */symtab_first_entry = symtab_entry;
		argdef();
		match(':');
		/* set subroutine type */symtab[entry].type = smptype();
    /* set var type as the same */symtab[entry_var].type = symtab[entry].type;
	}	
	/* assign all arguments declared in 'argdef' to subroutine */
	set_subroutine_argument_list(symtab_first_entry, symtab_entry, entry);
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
	/*sa*/int type;/**/
	if(lookahead == VAR)
	match(VAR);
	idlist();
	/*sa*/if(id_count > MAX_ARG_SIZE) {
		err(FATAL, LEXICAL, "Max indirections size overflow.\n");
	}/**/	
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
	/*sa*/satrb->indirections = 0;/**/
	/*sa*/satrb->type = 0;/**/ // CUIDADO CONFUNDIR COM PROCEDURE!!!
q0:
	if(lookahead == ARRAY) {
		match(ARRAY);
		match('[');		
		/*sa*/if(satrb->indirections > MAX_IND_SIZE) {
			err(FATAL, LEXICAL, "Max indirections size overflow.\n");
		}/**/
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
  int entry = symtab_retrieve(lexeme, 0);
	match(ID);
	if(lookahead == '(' || symtab[entry].attributes) {
    /* match entry attributes */param(entry);
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
  /* REMINDER: indexlevel is decremented as '[' is matched, and it may not
    overload indirections entries, but it can become lower */
q0:
  if(lookahead == '[') {
    indexlevel--;
    match('[');
    expr();
    match(']');
    goto q0;
  }  
  
  if(indexlevel < 0) {
    err(FATAL, SEMANTIC, "Too many index levels.\n");
  }
  
  return indexlevel >= 0;
}
/*
 * parm -> '(' [ ')' | exprlst ')']
 */
/**OBS: exprlst é opcional**/
param(int entry)
{	
  /* start paramindex */ int paramindex = symtab[entry].attributes;
  
  /* REMINDER: paramindex must match all attributes entries, in case if the
    subroutine has no arguments then it may match '()' or not (optional) */
  match('(');
  if(lookahead == ')') {
    match(')');
  } else {
    paramindex = exprlst(symtab[entry].attributes);
    match(')');
  }
  /* REMINDER: paramindex must be treated here and not in 'exprlst', because it
    may have none */    
  
  if(paramindex < 0) {
    err(FATAL, SEMANTIC, "Too many parameters.\n");
  } else if(paramindex > 0) {
    err(FATAL, SEMANTIC, "Too few parameters.\n");
  }
  
  return paramindex == 0;
}
/*
 * exprlst -> expr { ',' expr }
 */
int exprlst(int paramindex)
{	
   /* REMINDER: indexlevel is decremented as '[' is matched, and it may not
    overload indirections entrie, but it can become lower */
q0:
	expr();
  paramindex--;
  
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

  /* id symtab position */int entry;
	switch(lookahead) {
    case ID:
      /* search for id in symtab */entry = symtab_retrieve(lexeme, 1);
      match(ID);      
      if(lookahead == '(' || symtab[entry].attributes) {
        /* match entry attributes */param(entry);
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