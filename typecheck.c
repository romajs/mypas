#include <header/typecheck.h>

/* calculo de precedencia de operadores
 * Fonte: http://en.wikipedia.org/wiki/Order_of_operations#Programming_languages
 * OBS: quanto maior -> menos significativo */
precedence(const token_t token)
{
	switch (token) {
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

/* função que retorna um "valor amigável" dado o tipo semântico (de um operando)
 * OBS: o mesmo valor definido em "smptype" */
typev(token_t token)
{
	switch (token) {
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
			/* ERROR TYPE */
			return -1;
	}
}

/* função que verifica a compatibilidade entre dois tipos de operandos */
typecmp(int type1, int type2) {
	if (type1 <= 3 && type2 <= 3)
		return type1 > type2 ? type1 : type2;
	if (BOOL_CTE == type1 == type2)
		return BOOL_CTE;
	if (STR_CTE == type1 == type2 )
		return STR_CTE;
	return 0;
}

/* função que retorna quais os tipos suportados por um determinado operador */
int* semanticSupportedTypes(token_t op) {
    /* Order: INT, REAL, DBL, BOOL, STR, ex: {0,0,0,0,0} */
    switch (op) {
    case '+':
        return (int[]) {
            1,1,1,0,1
        }; // INT,REAL,DBL | STR
    case '-':
        return (int[]) {
            1,1,1,0,0
        }; // INT,REAL,DBL
    case '*':
        return (int[]) {
            1,1,1,0,0
        }; // INT,REAL,DBL
    case '/':
        return (int[]) {
            1,1,1,0,0
        }; // INT,REAL,DBL
    case DIV:
        return (int[]) {
            1,0,0,0,0
        }; // INT
    case MOD:
        return (int[]) {
            1,1,1,0,0
        }; // INT,REAL,DBL
    case AND:
        return (int[]) {
            0,0,0,1,0
        }; // BOOL
    case OR:
        return (int[]) {
            0,0,0,1,0
        }; // BOOL
    case EQ:
        return (int[]) {
            1,1,1,1,1
        }; // *
    case NEQ:
        return (int[]) {
            1,1,1,1,1
        }; // *
    case GRT:
        return (int[]) {
            1,1,1,0,0
        }; // INT,REAL,DBL
    case GEQ:
        return (int[]) {
            1,1,1,0,0
        }; // INT,REAL,DBL
    case LSR:
        return (int[]) {
            1,1,1,0,0
        }; // INT,REAL,DBL
    case LEQ:
        return (int[]) {
            1,1,1,0,0
        }; // INT,REAL,DBL
    }
}

/* função que calcula o resultado entre duas variávies dado seu operador */
double calc(double x, double y, token_t op) {
    double result = 0.00;
    switch (op) {
    case '+':
        result = x + y;
        break;
    case '-':
        result = x - y;
        break;
    case '*':
        result = x * y;
        break;
    case '/':
        result = x / y;
        break;
    case DIV:
        result = (int)(x / y);
        break;
    case MOD:
        result = (int)x % (int)y;
        break;
    case AND:
        result = x && y;
        break;
    case OR:
        result = x || y;
        break;
    case EQ:
        result = x == y;
        break;
    case NEQ:
        result = x != y;
        break;
    case GRT:
        result = x > y;
        break;
    case GEQ:
        result = x >= y;
        break;
    case LSR:
        result = x < y;
        break;
    case LEQ:
        result = x <= y;
        break;
    }
    return result;
}

/* função que empilha um 'operando' na pilha de operandos */
void push_operand(token_t token, const char *lexeme) {
	++sp;
	if(token == STR_CTE) {
		strcpy(operand[sp].lexeme, lexeme);
	} else {
		operand[sp].value = atof(lexeme);
	}
	operand[sp].type = typev(token);
}

/* função que empilha um 'operador' na pilha de operadores */
void push_oper(token_t symbol, int level) {
	++opsp;
	oper[opsp].symbol = symbol;
	oper[opsp].level = level;
}

/* função que verifica se deveria operar no momento */
can_oper(void) {
	/* pode operar desde que:
	 * 1 - caso os operadores estejam no mesmo nível de recursão
	 * 	1.1 - então verifica as precedências dos operadores
	 * 2 - tenha saído de um nível de recursão
	 */
	return (oper[opsp].level == oper[opsp - 1].level &&
				precedence(oper[opsp].symbol) <= precedence(lookahead)) ||
				oper[opsp].level > E_lvl;
}

/* função que executa as operações da pilha (desde que possa operar) */
void exec_oper(void) {
	/* deve operar quando: */
	if(opsp > - 1) { /* se houver ao menos um operador na pilha */
		while(can_oper()) { /* enquanto puder operar */

			/* 1 - verificar quais tipos semânticos o operador suporta */
			int *types = semanticSupportedTypes(oper[opsp].symbol);
			
			/* 2 - verificar se o tipo dos operandos bate com os suportados pelo operador */
			if(!types[operand[sp].type] || !types[operand[sp+1].type]) {
				err(WARN, SEMANTIC, "Incorrect type for operation \"%d\"\n",
				oper[opsp].symbol); 
			}
			
			/* 3 - verificar se os tipos entre os operandos são compatíveis */
			if(!typecmp(operand[sp].type, operand[sp+1].type)) {          
				err(WARN, SEMANTIC, "Operand types not compatible\n");
			}
			
			/* 4 - fazer o cáculo da operação */
			operand[--sp].value = calc(operand[sp].value, operand[sp+1].value, oper[opsp--].symbol); 
			
			// TODO: se termiar as operações no nível deve decrementá-lo
		} 
	}
}