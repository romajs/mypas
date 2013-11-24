/*@<symtab.c>*/
#include <string.h>
#include <stdlib.h>

#include <symtab.h>

SEMANTIC_ATTRIB symtab[MAX_SYMTAB_ENTRIES+1];

int symtab_entry = 1;

/* valida a próxima entrada a symtab, verificando a quantiddade já definida */
symtab_next_entry() {
  if(symtab_entry > MAX_SYMTAB_ENTRIES) {
    err(FATAL, OTHER, "Max variable declaration rechead.\n");
    return 0;
  } 
  return symtab_entry++;
}

symtab_reset_entries(int to) {
  memset(symtab + to, 0, sizeof(SEMANTIC_ATTRIB));
  symtab_entry = to;
}

/* faz uma busca na tabela de simbolos e retorna sua posição, caso "skipobj"
 seja true, a verificação irá pular os registros sem "objtype" definidos
 caso das variáveis referentes a manipulação do valor de retorno da função */
symtab_lookup(const char *query, int skipobj)
{
	debug("<symtab_lookup, entries = %d>\n", symtab_entry - 1);
	int i;
	for(i = symtab_entry - 1; i > 0; i--){
		debug_symtab_entry(i);		
    if(skipobj && !symtab[i].objtype) {
      debug("null objtype, skiping...");
      continue;
    }		
		if(strcmp(symtab[i].name, query) == 0) {      
      debug("%s founded in symtab at position %d.\n", query, i);
			return i;
		}
	}
  debug("%s not found.\n", query);
	return 0;
}

/* retorna uma variável já declarada */
symtab_retrieve(const char *query, int skipobj) {
	int entry = symtab_lookup(query, skipobj);
  if(!entry) {
    err(FATAL, SEMANTIC, "%s was not declared.\n", query);
  }
  return entry;
}

/* verifica se uma variável não existe */
symtab_validate(const char *query) {
	int entry = symtab_lookup(query, 0);
  if(entry) {
    err(FATAL, SEMANTIC, "%s already defined in current scope\n", query);
  }
  return entry;
}

/* adiciona uma lista de variáveis a tabela de símbolos (batch) */
symtab_add_list(int n, char symlist[MAX_SYMTAB_ENTRIES][MAX_ID_SIZE + 1],
  int type, int scope, int indirections, int dimension[MAX_IND_SIZE])
{
	debug("<symtab_add_list>\n");
	debug("n: %d\n", n);
	debug("type: %d\n", type);
	debug("scope: %d\n", scope);
	int i;
	for(i = 0; i < n; i++){
		debug("i: %d\n", i);
		debug("symlist[%d] = %s\n", i, symlist[i]);
    int entry = symtab_validate(symlist[i]);
    entry = symtab_next_entry();   
    /* set all struct atributes */
		strcpy(symtab[entry].name, symlist[i]);
		symtab[entry].type = type;
		symtab[entry].scope = scope;
    /* set objtype */if(symtab[entry].indirections) {
      symtab[entry].objtype = POINTER;
    } else {
      symtab[entry].objtype = ATOMIC;
    }    
		symtab[entry].attributes = 0;
		symtab[entry].indirections = indirections;
		memcpy(symtab[entry].dimension, dimension, sizeof(int) * MAX_IND_SIZE);
		debug("%s was added susessfully to symtab\n", symlist[i]);
		debug_symtab_entry(entry);
	}
  debug("</symtab_add_list>\n");
	return symtab_entry;
}

/* cadastra uma lista de variáveis (já declaradas em symtab) como argumentos
 de uma determinada subrotina (pos) */
void set_subroutine_argument_list(int from, int to, int pos) {
	debug("<set_subroutine_argument_list>\n");
	symtab[pos].argument = malloc((to - from) * sizeof(SEMANTIC_ATTRIB));
	symtab[pos].attributes = 0;
  int i;
	for(i = from; i < to; i++) {
    debug("adding argument to %s[%d]: ", symtab[pos].name,
      symtab[pos].attributes);
    debug_symtab_entry(i);
		memcpy(&symtab[pos].argument[symtab[pos].attributes++], &symtab[i],
      sizeof(SEMANTIC_ATTRIB));
	}
  debug("</set_subroutine_argument_list>\n");
}