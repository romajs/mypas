#include <header/lexer.h>

token_t lookahead;

char lexeme[MAX_ID_SIZE];

int lineno = 1;

/* evita caracteres em branco (espaços) & comentários */
void skipspaces(FILE *tape)
{
	token_t head;
q0:
	while(isspace(head=getc(tape))) {
		head == '\n' && lineno++;
		if(head == EOF) break;
	}
	if(head == '{') {
		while((head=getc(tape)) != '}') {
			head == '\n' && lineno++;
			if(head == EOF) break;
		}
		goto q0;
	}
	ungetc(head, tape);
}

/* fim de arquivo (EOF) */
token_t isEOF(FILE *tape)
{
	token_t head;
	if((head = getc(tape)) == EOF) {
		lexeme[0] = 0;
		return EOF;
	}
	ungetc(head, tape);
	return 0;
}

/* identificadores */
token_t isID(FILE *tape)
{
	token_t head;

	if (isalpha(head = getc(tape))) {
		int i = 0;
		lexeme[i++] = toupper(head);
		while (isalnum(head = getc(tape))) {
			if(i + 1 < MAX_ID_SIZE) {
				lexeme[i++] = toupper(head);
			}
		}
		lexeme[i] = 0;
		ungetc(head, tape);
		if(head = iskeyword(lexeme)) {
			return head;
		}
		return ID;
	}
	ungetc(head, tape);
	return 0;
}

/* contantes numéricas: integers ou ponto flutuante (double) */
token_t isNUM_CTE(FILE *tape)
{
	token_t head;
	int i = 0; /* lexeme position */
	int digits = 0;
	int precision = 0;
	int exponent = 0;
	
	if(isdigit(head = getc(tape))) {  /* parte inteira */
		digits++;
		lexeme[i++] = head;
		while(isdigit(head = getc(tape))){
			if(digits++ < MAX_INT_DIG) {
				lexeme[i++] = head;	
      }
		}
		if(head == '.') { /* parte fracionário (precisão) */
			lexeme[i++] = head;
			while(isdigit(head = getc(tape))){
				if(precision++ < MAX_PRC_DIG) {
					lexeme[i++] = head;
        }
			}
		}
	}
	if(toupper(head) == 'E') { /* parte exponencial */
		lexeme[i++] = head;	
		if((head = getc(tape)) == '+' || head == '-') {
			lexeme[i++] = head;	
			head = getc(tape);
		}
		if(isdigit(head)) {
			exponent++;
			lexeme[i++] = head;
			while(isdigit(head = getc(tape))){
				if(exponent++ < MAX_EXP_DIG) {
					lexeme[i++] = head;		
				}	
			}
		}
	}
	lexeme[i] = 0;
	ungetc(head, tape);
	if(precision > 6 || exponent > 8) {
		return DBL_CTE;
	} else if(precision || exponent) {
		return REAL_CTE;
	} else if(digits > 0) {		
		return INT_CTE;
	} else {	
		return 0;
	}
}

/* reconhecimento de cadeias de caracteres */
token_t isSTR_CTE(FILE *tape)
{
	token_t head;

	if((head = getc(tape)) == '\'') {
		int i = 0;
		while((head = getc(tape)) != '\''){			
			if(head == EOF) break;
			if(i + 1 < MAX_STR_LEN) {
				lexeme[i++] = head;		
			} else {
				err(FATAL, LEXICAL, "Max String Length Overflow\n");
			}
		}
		lexeme[i] = 0;
		return STR_CTE;
	}
	ungetc(head, tape);
	return 0;
}

/* atribuições / assignment */
token_t isATTR(FILE *tape)
{
	token_t t1, t2;

	if((t1 = getc(tape)) == ':') {
		int i = 0;
		lexeme[i++] = t1;	
		if((t2 = getc(tape)) == '=') {
			lexeme[i++] = t2;		
			lexeme[i] = 0;
			return ATTR;
		}
		ungetc(t2, tape);
	}
	ungetc(t1, tape);
	return 0;
}

/* operadores relacionais */
token_t isRELOP(FILE *tape)
{
	token_t t1, t2;
	int i = 0;
	if((t1 = getc(tape)) == '<' || t1 == '>' || t1 == '=') {
		lexeme[i++] = t1;	
		if((t2 = getc(tape)) == '=' || t2 == '>') {
			lexeme[i++] = t2;		
			lexeme[i] = 0;
			if (t1 == '=')
			return EQ;
			if(t2 == '>')
			return NEQ;
			if(t1 == '<')
			return LEQ;
			else
			return GEQ;
		}
		ungetc(t2, tape);
		if(t1 == '<')
		return LSR;
		else
		return GRT;
	}
	ungetc(t1, tape);
	return 0;
}

/* função que lê os caracteres da fita e tenta transformar em um símbolo da
 * gramática, para cada uma das funções acima (possíveis 'tokens')
 * caso nenhuma delas seja satisfeita apenas retorna o último caractere lido.
 */
token_t gettoken(FILE *tape)
{
	token_t token;  	

	skipspaces(tape);	
	
	if(token = isEOF(tape)) goto END_TOKEN;

	if (token = isID(tape)) goto END_TOKEN;

	if (token = isNUM_CTE(tape)) goto END_TOKEN;

	if (token = isSTR_CTE(tape)) goto END_TOKEN;

	if (token = isATTR(tape)) goto END_TOKEN;

	if (token = isRELOP(tape)) goto END_TOKEN;

	token = getc(tape);
	lexeme[0] = token;
	lexeme[1] = 0;  

	END_TOKEN:  	
	return token;
}
/*************************************************************************
 *** parser-to-lexer interface ********************************************
 *************************************************************************/

/* função que consome o 'token' atual e obtém o próximo 'token' da fita */
void match(token_t predicted) {

	if(lookahead == predicted) {
		/* importante: deve verificar se já não está em fim de arquivo, se
		 * não irá requisitar uma nova entrada de dados ao usuário */
		if(lookahead != EOF) {			
			lookahead = gettoken(source);
		}
	} else {		
		err(FATAL, LEXICAL, "Token mismatch.\n");
	}
}