/*@<symtab.c>*/
#include <string.h>
#include <stdlib.h>

#include <symtab.h>

SEMANTIC_ATTRIB symtab[MAX_SYMTAB_ENTRIES+1];

int symtab_next_entry = 1;

symtab_lookup(const char *query)
{
	debug("<symtab_lookup, entries = %d>\n", symtab_next_entry);
	int i;
	for(i = symtab_next_entry - 1; i > 0; i--){
		debug_symtab_entry(i);		
		if(strcmp(symtab[i].name, query) == 0) {
			return i;
		}
	}
	return 0;
}

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
		/* search for similiar entries */int entry = symtab_lookup(symlist[i]); 
		debug("entry: %d\n", entry);		    
		/* if there is any, goto error */if(entry && symtab[entry].scope >= scope){ 
			err(FATAL, SEMANTIC, "%s already defined in current scope\n", symlist[i]);
			return -1;
		}
    /* set all struct atributes */
		strcpy(symtab[symtab_next_entry].name, symlist[i]);
		symtab[symtab_next_entry].type = type;
		symtab[symtab_next_entry].scope = scope;
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

void set_subroutine_param_list(int from, int to, int pos) {
	debug("<symtab_asgm_params>\n");
	symtab[pos].param = malloc((to - from) * sizeof(SEMANTIC_ATTRIB));
	int i;
	for(i = from, symtab[pos].attributes = 0; i < to; i++, symtab[pos].attributes++) {
		memcpy(&symtab[pos].param[symtab[pos].attributes], &symtab[i], sizeof(SEMANTIC_ATTRIB));
	}
  debug("</symtab_asgm_params>\n");
}
