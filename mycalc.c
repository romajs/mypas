#include <mycalc.h>

// mulop -> '*' | '/' | DIV | MOD | AND
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

// addop -> '+' | '-' | OR
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

// relop -> '>' | '>=' | '<' | '<=' | '=' | '<>'
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

// http://en.wikipedia.org/wiki/Order_of_operations#Programming_languages
// quanto maior -> menos significativo
precedence(const token_t token)
{
	switch(token) {
    case NOT:
      return 2;
    case '*':
    case '/':
    case DIV:
    case MOD:
			return 3;
    case '+':
    case '-':
			return 4;
    case GRT:
    case GEQ:
    case LSR:
    case LEQ:
			return 6;
		case EQ:
    case NEQ:
			return 7;
    case AND:
			return 11;
    case OR:
			return 12;
  }
}

// função que retorna um valor amigável dado o tipo semântico (de um operando)
type(token_t token)
{
  debug("type\n");
	switch(token) {
		case INT_CTE:
			return 1;
		case REAL_CTE:
			return 2;
		case DBL_CTE:
			return 3;
		case BOOL_CTE:
			return 4;
		case STR_CTE:
			return 5;
		default:
      // ERROR TYPE
			return 0;
	}
}

// função que verifica a compatibilidade entre dois tipos (de operandos)
typeFactor(int type1, int type2) {
  if(type1 <= 3 && type2 <= 3)
    return type1 > type2 ? type1 : type2;
  if(BOOL_CTE == type1 == type2)
    return BOOL_CTE;
  if(STR_CTE == type1 == type2 )
    return STR_CTE;
  return 0;
}

// função que retorna quais os tipos suportados por um determinado operador
int* semanticSupportedTypes(token_t op) {
  // Order: INT, REAL, DBL, BOOL, STR, ex: {0,0,0,0,0}
  switch(op) {
		case '+': return (int[]) {1,1,1,0,1}; // INT,REAL,DBL | STR
		case '-': return (int[]) {1,1,1,0,0}; // INT,REAL,DBL
		case '*': return (int[]) {1,1,1,0,0}; // INT,REAL,DBL
		case '/': return (int[]) {1,1,1,0,0}; // INT,REAL,DBL
    case DIV: return (int[]) {1,0,0,0,0}; // INT
    case MOD: return (int[]) {1,1,1,0,0}; // INT,REAL,DBL
    case AND: return (int[]) {0,0,0,1,0}; // BOOL
    case OR:  return (int[]) {0,0,0,1,0}; // BOOL
    case EQ:  return (int[]) {1,1,1,1,1}; // *
    case NEQ: return (int[]) {1,1,1,1,1}; // *
    case GRT: return (int[]) {1,1,1,0,0}; // INT,REAL,DBL
    case GEQ: return (int[]) {1,1,1,0,0}; // INT,REAL,DBL
    case LSR: return (int[]) {1,1,1,0,0}; // INT,REAL,DBL
    case LEQ: return (int[]) {1,1,1,0,0}; // INT,REAL,DBL
	}
}

// função que calcula o resultado entre duas variávies dado seu operador
double calc(double x, double y, token_t op) { 
  double result = 0.00;
  switch(op) {
		case '+': result = x + y; break;
		case '-': result = x - y; break;
		case '*': result = x * y; break;
		case '/': result = x / y; break;
    case DIV: result = (int)(x / y); break;
    case MOD: result = (int)x % (int)y; break;
    case AND: result = x && y; break;
    case OR:  result = x || y; break;
    case EQ:  result = x == y; break;
    case NEQ: result = x != y; break;
    case GRT: result = x > y; break;
    case GEQ: result = x >= y; break;
    case LSR: result = x < y; break;
    case LEQ: result = x <= y; break;
	}
	debug( "(calc) %.2f %c %.2f = %.2f\n", x, op, y, result);
	return result;
}

// busca uma variável na tabela de símbolos e retorna sua posição
double recall(char const *variable) {
  // TODO:
	/*int i;
	for(i = 0; i <= nextentry; i++) {
    
		if(strcmp(SYMTAB[i], variable) == 0)
			return i;
	}
	err(FATAL, SYNTATIC, "Id not declared");*/
	return -1;
}

// busca uma variável na tabela de símbolos e retorna seu valor
double getvalue(char const *variable) {
  // TODO:
  /*int i;
  for(i = 0; i <= nextentry; i++) {
		if(strcmp(SYMTAB[i], variable) == 0)
			return acc[i];
	}
	err(FATAL, SYNTATIC, "Id not declared");*/
	return 0.00;
}

// função que empilha um 'operando' na pilha de operandos
void push_operand(token_t token, const char *lexeme) {
	++sp;
  if(token == STR_CTE) {
     strcpy(operand[sp].lexeme, lexeme);
  } else {
    operand[sp].value = atof(lexeme);
  }
  operand[sp].type = type(token);
	debug( "(push) operand[%d]: value = %.2f, type = %d\n", sp, 
    operand[sp].value, operand[sp].type);	
}

// função que empilha um 'operador' na pilha de operadores
void push_oper(token_t symbol, int level) {
	++opsp;
	oper[opsp].symbol = symbol;
	oper[opsp].level = level;
	debug( "(push) oper[%d]: symbol = \"%c\", level = %d\n", opsp,
    oper[opsp].symbol, oper[opsp].level);	
	//debug_oper(oper, E_lvl, opsp);
}

// função que verifica se deveria operar no momento
can_oper(void) {
	// pode operar desde que:
	// 1 - caso os operadores estejam no mesmo nível de recursão
	//     1.1 - então verifica as precedências dos operadores
	// 2 - tenha saído de um nível de recursão
	return (oper[opsp].level == oper[opsp - 1].level &&
				precedence(oper[opsp].symbol) <= precedence(lookahead)) ||
				oper[opsp].level > E_lvl;
}

// função que executa as operações da pilha (desde que possa operar)
void exec_oper(void) {
	// deve operar quando:
	if(opsp > - 1) { // se houver ao menos um operador na pilha
		while(can_oper()) { // enquanto puder operar			

      // 1 - verificar quais tipos semânticos o operador suporta
      int *types = semanticSupportedTypes(oper[opsp].symbol);
      
      // 2 - verificar se o tipo dos operandos bate com os suportados pelo operador
      if(!types[operand[sp].type] || !types[operand[sp+1].type]) {
        err(FATAL, SEMANTIC, "Incorrect type for operation \"%d\"\n",
          oper[opsp].symbol); 
        goto ERROR;
      }
      
      debug("(pop) oper[%d]: symbol = \"%c\", level = %d\n",
        opsp, oper[opsp].symbol, oper[opsp].level);	
      debug("(pop) operand[%d] = %.2f\n", sp, operand[sp]);
      debug("(pop) operand[%d] = %.2f\n", sp-1, operand[sp-1]);	
      
      // 3 - verificar se os tipos entre os operandos são compatíveis
      if(typeFactor(operand[sp].type, operand[sp+1].type)) {          
        err(FATAL, SEMANTIC, "Operand types not compatible\n");
        goto ERROR;
      }
      
      // 4 - fazer o cáculo da operação
      operand[--sp].value =
        calc(operand[sp].value, operand[sp+1].value, oper[opsp--].symbol); 
      
      // TODO: se termiar as operações no nível deve decrementá-lo
		} 
	}
ERROR:
  err(FATAL, SEMANTIC, "Incorret semantic operation.\n");
  // TODO:
}

Operand expr(void)
{ 
  debug("expr\n");
	memset(&oper[0], 0, sizeof(oper));
	memset(&operand[0], 0, sizeof(operand));	
	E_lvl = -1, R_lvl = -1, T_lvl = -1, F_lvl = -1;
	sp = -1, opsp = -1;

	E: debug( "E: %d\n", ++E_lvl);

	switch(lookahead) { // inversão de sinal ('-') e negação (NOT)
    case '-':
    case NOT:
      debug( "(signal reversion) activated.\n");
      //push_operand(lookahead, 0.00);
      //push_oper(lookahead, E_lvl);
      match(lookahead);
      break;
	}

	T: debug( "T: %d\n", ++T_lvl);	
  
  R: debug( "R: %d\n", ++R_lvl);

	F: debug( "F: %d\n", ++F_lvl); 	

	switch(lookahead) {
    case ID:
      match(ID);
		if(lookahead == '(') {
			param();
		} else {
			if(lookahead == '[') {
				match('[');
				expr();
				match(']');
			}
		}
      /*if(recall(lexeme) > -1) { 
        // deve empilhar o valor da variável de SYMTAB
        push_operand(getvalue(lexeme)); 
      } else { // caso contrário, a 'ID' não foi declarada
        err(FATAL, SYNTATIC, "Id not declared\n"); 
      }*/
      break;
    /**************************************************************************
     *** Constants defs goes next: INTEGER | REAL | DOUBLE | BOOLEAN | STRING *
     *************************************************************************/
    case TRUE: 
      //push_operand(TRUE, "1");
      match(TRUE);
      break;
    case FALSE:
      //push_operand(FALSE, "0");
      match(FALSE);
      break;
    case INT_CTE:
    case REAL_CTE:
    case DBL_CTE:
    case STR_CTE:
      // push_operand(lookahead, lexeme);
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
      err(FATAL, LEXICAL, "Token mismatch\n");
	}	
 
	_F: debug( "_F: %d\n", --F_lvl);
	
	//exec_oper();  
  
	if(isrelop(lookahead)) {
		push_oper(lookahead, E_lvl);
		match(lookahead);
		goto R;
	}
 
  _R: debug( "_R: %d\n", --R_lvl);
  
	//exec_oper();  

	if(ismulop(lookahead)) {
		push_oper(lookahead, E_lvl);
		match(lookahead);
		goto F;
	}

	_T: debug( "_T: %d\n", --T_lvl);

	//exec_oper(); 

	if(isaddop(lookahead)) {
		push_oper(lookahead, E_lvl);
		match(lookahead);
		goto T;
	}

	_E: debug( "_E: %d\n", --E_lvl);

	if(E_lvl > -1) {
		match(')');
		goto _F;
	}

	debug( "(pop) operand[%d] = %.2f\n", sp, operand[sp]);
	return operand[sp--];
}