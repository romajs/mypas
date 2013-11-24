/*@<symtab.c>*/
#include <string.h>
#include <stdlib.h>

#include <symtab.h>

SEMANTIC_ATTRIB symtab[MAX_SYMTAB_ENTRIES+1];

int symtab_next_entry = 1;

// faz uma busca na tabela de simbolos e retorna sua posição
symtab_lookup(const char *query, int skipobj)
{
	debug("<symtab_lookup, entries = %d>\n", symtab_next_entry - 1);
	int i;
	for(i = symtab_next_entry - 1; i > 0; i--){
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

// retorna uma variável já declarada
symtab_retrieve(const char *query, int skipobj) {
	int entry = symtab_lookup(query, skipobj);
  if(!entry) {
    err(FATAL, SEMANTIC, "%s was not declared.\n", query);
  }
}

// verifica se uma variável não existe
symtab_validate(const char *query) {
	int entry = symtab_lookup(query, 0);
  if(entry) {
    err(FATAL, SEMANTIC, "%s already defined in current scope\n", query);
  }
}

// adiciona uma lista de variáveis a tabela de símbolos (batch)
symtab_add_list(int n, char symlist[MAX_SYMTAB_ENTRIES][MAX_ID_SIZE + 1], int type, int scope,
	int indirections, int dimension[MAX_IND_SIZE])
{
	debug("<symtab_add_list>\n");
	debug("n: %d\n", n);
	debug("type: %d\n", type);
	debug("scope: %d\n", scope);
	int i;
	for(i = 0; i < n; i++){
		debug("i: %d\n", i);
		debug("symlist[%d] = %s\n", i, symlist[i]);
		/* search for similiar entries */int entry = symtab_lookup(symlist[i], 0); 
		debug("entry: %d\n", entry);		    
		/* if there is any, goto error */if(entry && symtab[entry].scope >= scope){ 
			err(FATAL, SEMANTIC, "%s already defined in current scope\n", symlist[i]);
			return -1;
		}
    /* set all struct atributes */
		strcpy(symtab[symtab_next_entry].name, symlist[i]);
		symtab[symtab_next_entry].type = type;
		symtab[symtab_next_entry].scope = scope;
    /* set objtype */if(symtab[symtab_next_entry].indirections) {
      symtab[symtab_next_entry].objtype = POINTER;
    } else {
      symtab[symtab_next_entry].objtype = ATOMIC;
    }    
		symtab[symtab_next_entry].attributes = 0;
		symtab[symtab_next_entry].indirections = indirections;
		memcpy(symtab[symtab_next_entry].dimension, dimension, sizeof(int) * MAX_IND_SIZE);
		debug("%s was added susessfully to symtab\n", symlist[i]);
		debug_symtab_entry(symtab_next_entry);
		symtab_next_entry++;
	}
  debug("</symtab_add_list>\n");
	return symtab_next_entry;
}

// cadastra uma lista de variáveis (já declaradas em symtab) como argumentos
// de uma determinada subrotina (pos)
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