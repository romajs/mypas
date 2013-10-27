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
		while (isalnum(head = getc(tape)) && i + 1 < MAX_ID_SIZE) {
      lexeme[i++] = toupper(head);
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
token_t isNUM(FILE *tape)
{
	token_t head;

	if(isdigit(head = getc(tape))) {
		int i = 0;
		lexeme[i++] = head;
		while(isdigit(head = getc(tape))){
			if(i + 1 < MAX_INT_DIG)
				lexeme[i++] = head;		
		}
		if(head == '.') {
			lexeme[i++] = '.';
			while(isdigit(head = getc(tape))){
				if(i + 1 < MAX_PRC_DIG) {
					lexeme[i++] = head;
				} else {
					//exit_with_error(MAX_DIG_OVERFLOW);
				}
				// TODO: terminar PONTO FLUTUANTE
			}
		}
		lexeme[i] = 0;
		ungetc(head,tape);
		return NUM;
	}
	ungetc(head, tape);
	return 0;
}

// função que lê os caracteres da fita e tenta transformar em um símbolo da
// gramática, para cada uma das funções acima (possíveis 'tokens')
// caso nenhuma delas seja satisfeita apenas retorna o último caractere lido.
token_t gettoken(FILE *tape)
{
	token_t token;  
  debug("gettoken, ");

	if(token = isEOF(tape)) goto END_TOKEN;

	skipspaces(tape);

	if (token = isID(tape)) goto END_TOKEN;

	if (token = isNUM(tape)) goto END_TOKEN;

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
  debug("match, ");
  debug("lookahead: %d, ", lookahead);
  debug("predicted: %d, ", predicted);
  debug("lexeme: \"%s\", ", lexeme);
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
	debug( "(unmatch) previous = %d, lexeme = \"%s\"\n", lookahead, lexeme);
	int i;
	if(lookahead != EOF) {
		debug( "size(%s) = %u, ungetc: \"", lexeme, strlen(lexeme));
		for(i = strlen(lexeme) - 1; i >= 0; i--) {
			debug( "%c", lexeme[i]);
			ungetc(lexeme[i], source);
		}    
		debug( "\"\n");  
	} else {
		ungetc('\n', source);
	}
	lookahead = previous;
	strcpy(lexeme, temp);
	debug( "lookahead = %d, lexeme = \"%s\"\n", lookahead, lexeme);
}