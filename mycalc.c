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
	debug( "(push) operand[%d] = %.2f\n", sp, operand[sp]);	
}

// função que empilha um 'operador' na pilha de operadores
void push_oper(token_t token) {
	++opsp;
	oper[opsp].symbol = token;
	oper[opsp].level = E_lvl;
	//debug( "(push) oper[%d][%d] = \"%c\"\n", E_lvl, opsp, oper[E_lvl][opsp]);	
	//debug_oper(oper, E_lvl, opsp);
}

// função que verifica se deveria operar no momento
should_oper(void) {
	// pode operar desde que:
	return oper[opsp].level == oper[opsp - 1].level &&
				precedence(oper[opsp].symbol) <= precedence(lookahead);
}

// função que executa as operações da pilha (desde que possa operar)
void exec_oper(void) {
	// deve operar quando:
	if(opsp > - 1) { // se puder operar
		do {			
			debug( "(pop) oper[%d][%d] = \"%c\"\n", E_lvl, opsp, oper[opsp].symbol);	
			operand[--sp] = calc(operand[sp], operand[sp+1], oper[opsp--].symbol);
			debug( "(pop) operand[%d] = %.2f\n", sp, operand[sp]);	
		} while(can_oper = should_oper()); // enquanto puder operar
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

	can_oper = 0;

	if(lookahead == '-') { // inversão de sinal
		match('-');
		debug( "(signal reversion) activated.\n");
		push_operand(0);
		push_oper('-');
		can_oper = 1; // deve fazer esta operação o quanto antes (imediato)    
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
        push_operand(getvalue(lexeme)); // deve empilhar o valor da variável de SYMTAB
      } else {
        err(FATAL, SYNTATIC, "Id not declared\n"); // caso contrário, a 'ID' não foi declarada
      }*/
      break;
    case NUM: // INT | REAL | DOUBLE
      push_operand(atof(lexeme)); // empilha o valor da constante
      match(NUM);
      break;
    case STR:
      match(STR);
      break;
    case '(':
      match('(');
      goto E;
    default:
      // se não for nenhum dos esperados acima, então não faz parte da gramática
      err(FATAL, LEXICAL, "Token mismatch\n");
	}	
 
	_F: debug( "_F: %d\n", --F_lvl);
	
		exec_oper();  
  
	if(isrelop(lookahead)) {
		push_oper(lookahead);
		match(lookahead);
		goto R;
	}
 
  _R: debug( "_R: %d\n", --R_lvl);
  
	exec_oper();  

	if(ismulop(lookahead)) {
		push_oper(lookahead);
		match(lookahead);
		can_oper = 1;
		goto F;
	}

	_T: debug( "_T: %d\n", --T_lvl);

	// operações de '+' ou '-' são 'agendadas'
	// Não precisa atribuir 'can_oper = 0;' pois já é feito automaticamente em exe_oper() 

	if(isaddop(lookahead)) {
		push_oper(lookahead);
		match(lookahead);
		goto T;
	}

	// se chegou até É porque OU é fim de arquivo OU é ')', então:
	// 1 - deve operar o restante da pilha
	can_oper = 1;
	exec_oper();  

	_E: debug( "_E: %d\n", --E_lvl);

	if(E_lvl > -1) {
		match(')');
		goto _F;
	}

	debug( "(pop) operand[%d] = %.2f\n", sp, operand[sp]);
	return operand[sp--];
}