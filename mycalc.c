#include <mycalc.h>

// função que calcula o resultado entre duas variávies dado seu operador
double calc(double x, double y, int op) { 
  double result = 0.00;
  switch(op) {
		case '+': result = x + y; break;
		case '-': result = x - y; break;
		case '*': result = x * y; break;
		case '/': result = x / y; break;
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
	oper[E_lvl][++opsp] = token;
	debug( "(push) oper[%d][%d] = \"%c\"\n", E_lvl, opsp, oper[E_lvl][opsp]);	
	debug_oper(oper, E_lvl, opsp);
}

// função que verifica se é possível operar no momento
should_oper(void) {
	// pode operar desde que:
	//	1 - enquanto houverem operadoes;
	//	2 - enquanto o próximo operador não for '*' ou '/' (mulop);
	return oper[E_lvl][opsp] && !(lookahead == '*' || lookahead == '/');
}

// função que executa as operações da pilha (desde que possa operar)
void exec_oper(void) {
	// deve operar quando:
	// 1 - houver ao menos 1 operador, opsp = 0
	// 2 - houver mais que 1 operando, sp = 1 , para poder calcular (sp0 + sp1)
	// 3 - can_oper for verdadeiro (mulop | EOF | ')'
	if(opsp > - 1 && sp > -1 && oper[E_lvl][opsp] && can_oper) { // se puder operar
		do {			
			debug( "(pop) oper[%d][%d] = \"%c\"\n", E_lvl, opsp, oper[E_lvl][opsp]);	
			operand[--sp] = calc(operand[sp], operand[sp+1], oper[E_lvl][opsp--]);
			debug( "(pop) operand[%d] = %.2f\n", sp, operand[sp]);	
		} while(can_oper = should_oper()); // enquanto puder operar
	} 
	debug_oper(oper, E_lvl, opsp);
}

double expr(void)
{ 
  debug("expr\n");
	memset(&oper[0], 0, sizeof(oper));
	memset(&operand[0], 0, sizeof(operand));	
	E_lvl = -1, T_lvl = -1, F_lvl = -1;
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

	F: debug( "F: %d\n", ++F_lvl);  

	switch(lookahead) {
    case ID:
      match(ID);
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

	exec_oper(); // executa as operações mais 'voláteis' ('*' & '/')	 

	if(lookahead == '*' || lookahead == '/') {
		push_oper(lookahead);
		match(lookahead);
		can_oper = 1;
		goto F;
	}

	_T: debug( "_T: %d\n", --T_lvl);

	// operações de '+' ou '-' são 'agendadas'
	// Não precisa atribuir 'can_oper = 0;' pois já é feito automaticamente em exe_oper() 

	if(lookahead == '+' || lookahead == '-') {
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