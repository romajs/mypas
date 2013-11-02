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

type(token_t token)
{
  debug("type\n");
	switch(token) {
		case INTEGER:
			return 1;
		case REAL:
			return 2;
		case DOUBLE:
			return 3;
		case BOOLEAN:
			return 4;
		case STRING:
			return 5;
		default:
			return -1;
	}
}

// função que calcula o resultado entre duas variávies dado seu operador
double calc(double x, double y, int op) { 
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
void push_operand(double value) {
	operand[++sp] = value;
	debug( "(push) operand[%d]: value = %.2f\n", sp, operand[sp]);	
}

// função que empilha um 'operador' na pilha de operadores
void push_oper(token_t token) {
	++opsp;
	oper[opsp].symbol = token;
	oper[opsp].level = E_lvl;
	debug( "(push) oper[%d]: symbol = \"%c\", level = %d\n", opsp, token, E_lvl);	
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
	if(opsp > - 1) { // se puder operar
		while(can_oper()) { // enquanto puder operar			
			debug("(pop) oper[%d]: symbol = \"%c\", level = %d\n",
        opsp, oper[opsp].symbol, oper[opsp].level);	
			debug("(pop) operand[%d] = %.2f\n", sp, operand[sp]);
			debug("(pop) operand[%d] = %.2f\n", sp-1, operand[sp-1]);	
			operand[--sp] = calc(operand[sp], operand[sp+1], oper[opsp--].symbol);
		} 
	} 
	//debug_oper(oper, E_lvl, opsp);
}

double expr(void)
{ 
  debug("expr\n");
	memset(&oper[0], 0, sizeof(oper));
	memset(&operand[0], 0, sizeof(operand));	
	E_lvl = -1, R_lvl = -1, T_lvl = -1, F_lvl = -1;
	sp = -1, opsp = -1;

	E: debug( "E: %d\n", ++E_lvl);

	if(lookahead == '-') { // inversão de sinal
		match('-');
		debug( "(signal reversion) activated.\n");
		push_operand(0);
		push_oper('-');
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
      //push_operand(1.00);
      match(TRUE);
      break;
    case FALSE:
      //push_operand(0.00);
      match(FALSE);
      break;
    case INT_CTE:
    case REAL_CTE:
    case DBL_CTE:
      //push_operand(atof(lexeme)); // empilha o valor da constante
      match(lookahead);
      break;
    case STR_CTE:
      // TODO: push_operand(???);
      match(STR_CTE);
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
		push_oper(lookahead);
		match(lookahead);
		goto R;
	}
 
  _R: debug( "_R: %d\n", --R_lvl);
  
	//exec_oper();  

	if(ismulop(lookahead)) {
		push_oper(lookahead);
		match(lookahead);
		goto F;
	}

	_T: debug( "_T: %d\n", --T_lvl);

	//exec_oper(); 

	if(isaddop(lookahead)) {
		push_oper(lookahead);
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