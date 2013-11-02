#include <lexer.h>

token_t lookahead;

char lexeme[MAX_ID_SIZE];

int lineno = 1;

// evita caracteres em branco (espaços) & comentários
void skipspaces(FILE *tape)
{
	token_t head;
q0:
	while(isspace(head=getc(tape))) {
		head == '\n' && lineno++;
	}
	if(head == '{') {
		while((head=getc(tape)) != '}') {
			head == '\n' && lineno++;
		}
		goto q0;
	}
	ungetc(head, tape);
}

// fim de arquivo (EOF)
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

// identificadores
token_t isID(FILE *tape)
{
	token_t head;

	if (isalpha(head = getc(tape))) {
		int i = 0;
		lexeme[i++] = toupper(head);
		while (isalnum(head = getc(tape))) {
      if(i + 1 < MAX_ID_SIZE) {
        lexeme[i++] = toupper(head);
      } else {
        err(FATAL, LEXICAL, "Max Integer Digits Overflow\n");
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

// contantes numéricas: integers ou ponto flutuante (double)
token_t isNUM_CTE(FILE *tape)
{
	token_t head;

	if(isdigit(head = getc(tape))) {
		int i = 0;
		lexeme[i++] = head;
		while(isdigit(head = getc(tape))){
			if(i + 1 < MAX_INT_DIG) {
				lexeme[i++] = head;		
      } else {
        err(FATAL, LEXICAL, "Max Integer Digits Overflow\n");
      }
		}
		if(head == '.') {
			lexeme[i++] = '.';
			while(isdigit(head = getc(tape))){
				if(i + 1 < MAX_PRC_DIG) {
					lexeme[i++] = head;
				} else {
					err(FATAL, LEXICAL, "Max Precision Digits Overflow\n");
				}
				// TODO: terminar PONTO FLUTUANTE
			}
      // TODO: e quanto a REAL_CTE ???
      lexeme[i] = 0;
      ungetc(head,tape);
      return DBL_CTE;
		}
		lexeme[i] = 0;
		ungetc(head,tape);
		return INT_CTE;
	}
	ungetc(head, tape);
	return 0;
}

/*token_t isTRUE(FILE *tape)
{
	token_t head;
	int i = 0;
	const char* s = "TRUE";
	while(i < sizeof(s)) {
		head = getc(tape);
		lexeme[i++] = head;
		if(toupper(head) != s[i]) {
			while(i > 0)
				ungetc(lexeme[--i], tape);
			return 0;
		}
	}
	lexeme[i] = 0;
	return TRUE;
}

token_t isFALSE(FILE *tape)
{
	token_t head;
	int i = 0;
	const char* s = "FALSE";
	while(i < sizeof(s)) {
		head = getc(tape);
		lexeme[i++] = head;
		if(toupper(head) != s[i]) {
			while(i > 0)
				ungetc(lexeme[--i], tape);
			return 0;
		}
	}
	lexeme[i] = 0;
	return FALSE;
}*/

token_t isSTR_CTE(FILE *tape)
{
	token_t head;

	if((head = getc(tape)) == '\'') {
		int i = 0;
		while((head = getc(tape)) != '\''){
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

// função que lê os caracteres da fita e tenta transformar em um símbolo da
// gramática, para cada uma das funções acima (possíveis 'tokens')
// caso nenhuma delas seja satisfeita apenas retorna o último caractere lido.
token_t gettoken(FILE *tape)
{
	token_t token;  
  debug("gettoken: ");

	if(token = isEOF(tape)) goto END_TOKEN;

	skipspaces(tape);

	if (token = isID(tape)) goto END_TOKEN;

	if (token = isNUM_CTE(tape)) goto END_TOKEN;
  
  if (token = isSTR_CTE(tape)) goto END_TOKEN;
	
	//if (token = isTRUE(tape)) goto END_TOKEN;
	
	//if (token = isFALSE(tape)) goto END_TOKEN;
  
  if (token = isATTR(tape)) goto END_TOKEN;
  
  if (token = isRELOP(tape)) goto END_TOKEN;

	token = getc(tape);
	lexeme[0] = token;
	lexeme[1] = 0;  

END_TOKEN:  
	debug("lookahead: %d, ", token);
  debug("lexeme: \"%s\"\n", lexeme); 
	return token;
}
/**************************************************************************
 *** parser-to-lexer interface ********************************************
 *************************************************************************/

// função que consome o 'token' atual e obtém o próximo 'token' da fita
void match(token_t predicted) {
  debug("match? ");
  debug("lookahead: %d, ", lookahead);
  debug("predicted: %d, ", predicted);
  debug("lexeme: \"%s\" ", lexeme);
  if(lookahead == predicted) {
    if(lookahead != EOF) {
      debug("MATCH!\n");
      lookahead = gettoken(source);
    } else {
      debug("No more tokens in tape to get!\n");
     }
  } else {
    debug("FAILED!\n");
    err(FATAL, LEXICAL, "Token mismatch.\n");
  }
}

// função que devole o 'token' & 'lexeme' atual, e os redefine (volta ao anterior)
void unmatch(token_t previous, const char* temp) {
	debug("unmatch: { ");  
  debug("previous: %d, ", previous);
  debug("lexeme: \"%s\"; ", temp);
  debug("lookahead: %d, ", lookahead);
  debug("lexeme: \"%s\"} ", lexeme);
	int i;
	if(lookahead != EOF) {
		for(i = strlen(lexeme) - 1; i >= 0; i--) {
			ungetc(lexeme[i], source);
		}    		
	} else {
		ungetc('\n', source);
	}
	lookahead = previous;
	strcpy(lexeme, temp);
  debug("DONE!\n");  
  debug("result: { "); 
	debug("lookahead: %d, ", lookahead);
  debug("lexeme: \"%s\"} ", lexeme);
}