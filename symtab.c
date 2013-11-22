/*@<symtab.c>*/
#include <string.h>

#include <symtab.h>

SEMANTIC_ATTRIB symtab[MAX_SYMTAB_ENTRIES+1];

int symtab_next_entry = 1;

symtab_lookup(const char *query)
{
	debug("symtab_lookup\n");
	debug("symtab_next_entry = %d\n", symtab_next_entry);
	int i;
	for(i = symtab_next_entry - 1; i > 0; i--){
		debug("symtab[%d]: { name = %s, scope = %d, type = %d, ind = %d, attr = %d }\n",
			i, symtab[i].name, symtab[i].scope, symtab[i].type, symtab[i].indirections,
			symtab[i].attributes);
		int j;
		for(j = 0; j < symtab[i].attributes; j++) {
			debug("\t[%d]: { name = %s, scope = %d, type = %d, ind = %d }\n",
				j, symtab[i].param[j].name, symtab[i].param[j].scope, symtab[i].param[j].type,
				symtab[i].param[j].indirections);
		}
		if(symtab[i].indirections) {
			debug("\tdimensions: ");
			debug("%d ", symtab[i].dimension[0]);
			for(j = 1; j < symtab[i].indirections; j++) {
				debug("x %d ", symtab[i].dimension[j]);
			}
			debug("\n: ");
		}
		
		if(strcmp(symtab[i].name, query) == 0) {
			return i;
		}
	}
	return 0;
}

symtab_add_list(int n, char symlist[MAX_SYMTAB_ENTRIES][MAX_ID_SIZE + 1], int type, int scope)
{
	debug("symtab_add_list\n");
	debug("n: %d\n", n);
	debug("type: %d\n", type);
	debug("scope: %d\n", scope);
	int i;
	for(i = 0; i < n; i++){
		debug("i: %d\n", i);
		debug("symlist[%d] = %s\n", i, symlist[i]);
		int entry = symtab_lookup(symlist[i]); // busca pela variavel declarada (lexema)
		debug("entry: %d\n", entry);		
		if(entry && symtab[entry].scope >= scope){ // verifica se já existe
			err(FATAL, SEMANTIC, "%s already defined in current scope\n", symlist[i]);
			return -1;
		}
		strcpy(symtab[symtab_next_entry].name, symlist[i]);
		symtab[symtab_next_entry].type = type;
		symtab[symtab_next_entry].scope = scope;
		symtab[symtab_next_entry].attributes = 0;
		symtab[symtab_next_entry].indirections = 0;
		symtab_next_entry++;
	}
	return symtab_next_entry;
}