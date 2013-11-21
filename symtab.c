/*@<symtab.c>*/
#include <string.h>

#include <symtab.h>

SEMANTIC_ATTRIB symtab[MAX_SYMTAB_ENTRIES+1];

int symtab_next_entry = 1;

symtab_lookup(const char *query)
{
	debug("symtab_lookup\n");
	int i;
	for(i = symtab_next_entry - 1; i > 0; i--){
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
		int entry = symtab_lookup(symlist[i]);
		debug("entry: %d\n", entry);
		if(entry && symtab[entry].scope >= scope){
			err(FATAL, SEMANTIC, "%s already defined in current scope\n", symlist[i]);
			return -1;
		}
		strcpy(symtab[symtab_next_entry].name, symlist[i]);
		symtab[symtab_next_entry].type = type;
		symtab[symtab_next_entry].scope = scope;
		symtab_next_entry++;
	}
	return symtab_next_entry;
}