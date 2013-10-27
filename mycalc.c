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
// caso não existe, então define
double recall(char const *variable) {
	int i;
	for(i = 0; i <= nextentry; i++) {
		if(strcmp(SYMTAB[i], variable) == 0)
			return i;
	}
	if(nextentry + 1 > MAX_SYMTAB_ENTRIES) {
		err(FATAL, OTHER, "Max ID declaration reached\n");
	}  
	//strcpy(SYMTAB[++nextentry], variable); // adiciona a variável em SYMTAB
	return -1;
}

// busca uma variável na tabela de símbolos e retorna seu valor de acc
double getvalue(char const *variable) {
  int i;
  for(i = 0; i <= nextentry; i++) {
		if(strcmp(SYMTAB[i], variable) == 0)
			return acc[i];
	}
	err(FATAL, SYNTATIC, "Id not declared");
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
	// 3 - não seja uma atribuição '=' (attr)
	return oper[E_lvl][opsp] && !(lookahead == '*' || lookahead == '/') &&
		oper[E_lvl][opsp] != '=';
}

// função que executa as operações da pilha (desde que possa operar)
void exec_oper(void) {
	// deve operar quando:
	// 1 - houver ao menos 1 operador, opsp = 0
	// 2 - houver mais que 1 operando, sp = 1 , para poder calcular (sp0 + sp1)
	// 3 - can_oper for verdadeiro (mulop | EOF | ')'
	// 4 - operador não pode ser de atribuição
	if(opsp > - 1 && sp > -1 && oper[E_lvl][opsp] && can_oper && oper[E_lvl][opsp] != '=') { // se puder operar
		do {			
			debug( "(pop) oper[%d][%d] = \"%c\"\n", E_lvl, opsp, oper[E_lvl][opsp]);	
			operand[--sp] = calc(operand[sp], operand[sp+1], oper[E_lvl][opsp--]);
			debug( "(pop) operand[%d] = %.2f\n", sp, operand[sp]);	
		} while(can_oper = should_oper()); // enquanto puder operar
	} 
	debug_oper(oper, E_lvl, opsp);
}

// função responsável por realizar as atribuições
void exec_attr(void) {
	double value;
	// deve atribuir quando:
	// 1 - houverem operadores '=' na pilha
	while(opsp > -1 && oper[E_lvl][opsp] == '=') {
		debug( "(pop) oper[%d][%d] = \"%c\"\n", E_lvl, opsp, oper[E_lvl][opsp--]);

		if(sp > -1) { 
			// obtém valor do operador no topo da fita
			// obs: se não houverem mais operandos, mantém o anterior
			value = operand[sp];
		}    
		debug("value = %.2f\n", value);

		debug( "(pop) attr[%d] = %d\n", asp, attr[asp]);
		int attr_pos = attr[asp--];

		debug("attr_pos = %d, lexeme = \"%s\".\n", attr_pos, SYMTAB[attr_pos]);

		acc[attr_pos] = value;
		debug( "acc[%d] = %.2f\n", attr_pos, value);    
	} 
	debug_oper(oper, E_lvl, opsp);
}

/*
 * LL(1) expression grammar
 *
 * EBNF:
 *
 *
 * expr -> {attr} [-] term { [+|-] term }
 *
 *        -----(A)-----    --(+|-)--
 *        |   -(-)-   |    |       |
 *        |   |   |   |    |       |
 *        v   |   v   |    v       |
 * (E)------------------->(T)--------->(_E)
 *
 * 
 * term -> fact { [*|/] fact }
 *
 *             --(*|/)--
 *             |       |
 *             v       |
 * (T)------->(F)--------->(_T)
 * 
 * 
 * fact -> ID ['=' expr] | NUM | '('expr')'
 *
 * (F)---------------> (ID) --------------\
 *  | \                                    \
 *  |  \------------> (NUM) ------------->(_F)
 *  |                                      /
 *   \-----> '(' ----> (E) ----> ')' -----/
 * 
 *
 * attr -> ID '='
 *
 * (A)------>(ID)------>'='------>(_A)
 *
 */
double expr(void)
{ 
	memset(&attr[0], 0, sizeof(attr));	
	memset(&oper[0], 0, sizeof(oper));
	memset(&operand[0], 0, sizeof(operand));	
	E_lvl = -1, T_lvl = -1, F_lvl = -1, A_lvl = -1;
	sp = -1, opsp = -1, asp = -1;
	int can_attr = 0; // flag de atribuição

	E: debug( "E: %d\n", ++E_lvl);

	can_oper = 0;
	can_attr = 1;

	A: debug( "A: %d\n", ++A_lvl);
	char variable[MAX_ID_SIZE];
	int attr_pos = -1;

	if(lookahead == '-') { // inversão de sinal
		match('-');
		debug( "(signal reversion) activated.\n");
		push_operand(0);
		push_oper('-');
		can_oper = 1; // deve fazer esta operação o quanto antes (imediato)    
	}

	T: debug( "T: %d\n", ++T_lvl);

	F: debug( "F: %d\n", ++F_lvl);  

	if(lookahead == ID) {
		strcpy(variable, lexeme); // faz uma cópia do lexema de 'ID' 
		
		match(ID);  
		
		attr_pos = recall(variable); // busca a posicação em SYMTAB caso exista, caso contrário = -1
		
		if(lookahead != '=') { // se não for declaração
			if(attr_pos > -1) {
				push_operand(getvalue(variable)); // deve empilhar o valor da variável de SYMTAB
			} else {
				err(FATAL, SYNTATIC, "Id not declared\n"); // caso contrário, a 'ID' não foi declarada
			}
		}
	} else if(lookahead == NUM) {
		push_operand(atof(lexeme)); // empilha o valor da constante
		match(NUM);
	} else if(lookahead == '(') {
		match('(');
		goto E;
	} else { // se não for nenhum dos esperados acima, então não faz parte da gramática
		err(FATAL, LEXICAL, "Token mismatch\n");
	}	

	if(lookahead == '=' && can_attr) { // atribuição
		if(attr_pos == -1) {
			strcpy(SYMTAB[++nextentry], variable); // insere em SYMTAB
			attr_pos = nextentry;
			debug( "(attrd) SYMTAB[%d] = \"%s\".\n", attr_pos, variable);  
		} else {
			debug( "(retvd) SYMTAB[%d] = \"%s\".\n", attr_pos, variable);  
		}
		attr[++asp] = attr_pos; // empilha a posição da ID de SYMTAB em attr
		debug( "(push) attr[%d] = %d\n", asp, attr[asp]);	
		push_oper('=');       
		match('=');    
		goto A;
	}

	_A: debug( "A: %d\n", --A_lvl);

	can_attr = 0;

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
	// 2 - deve realizar todas as astribuições do nível de recursão atual (E_lvl)
	can_oper = 1;
	exec_oper();  
	exec_attr();

	_E: debug( "_E: %d\n", --E_lvl);

	if(E_lvl > -1) {
		match(')');
		goto _F;
	}

	match(EOF); 

	// no caso de iniciar com '-' antes de 'attr'
	if(opsp > -1 && oper[0][opsp] == '-') {
		operand[sp] *= -1;
	}

	debug( "(pop) operand[%d] = %.2f\n", sp, operand[sp]);
	return operand[sp--];
}