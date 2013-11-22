/*@<symtab.c>*/
#include <string.h>
#include <stdlib.h>

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
		int j = 0;
		if(symtab[i].indirections) {
			debug("\tdimensions: ");
			debug("%d ", symtab[i].dimension[j]);
			for(j = 1; j < symtab[i].indirections; j++) {
				debug("x %d ", symtab[i].dimension[j]);
			}
			debug("\n");
		}
		for(j = 0; j < symtab[i].attributes; j++) {
			debug("\t[%d]: { name = %s, scope = %d, type = %d, ind = %d }\n",
				j, symtab[i].param[j].name, symtab[i].param[j].scope, symtab[i].param[j].type,
				symtab[i].param[j].indirections);
		}
		
		if(strcmp(symtab[i].name, query) == 0) {
			return i;
		}
	}
	return 0;
}

symtab_add_list(int n, char symlist[MAX_SYMTAB_ENTRIES][MAX_ID_SIZE + 1], int type, int scope,
	int indirections, int dimension[MAX_IND_SIZE])
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
		symtab[symtab_next_entry].indirections = indirections;
		memcpy(symtab[symtab_next_entry].dimension, dimension, sizeof(dimension));
		debug("added to symtab[%d]: { name = %s, scope = %d, type = %d, ind = %d, attr = %d }\n",
			symtab_next_entry, symtab[symtab_next_entry].name, symtab[symtab_next_entry].scope,
			symtab[symtab_next_entry].type, symtab[symtab_next_entry].indirections, symtab[symtab_next_entry].attributes);
		symtab_next_entry++;
	}
	return symtab_next_entry;
}

int symtab_add(SEMANTIC_ATTRIB *satrb)
{
	debug("<symtab_add>\n");	
	int entry = symtab_lookup(satrb->name); // busca pela variavel declarada (lexema)
	debug("entry: %d\n", entry);		
	if(entry && symtab[entry].scope >= satrb->scope){ // verifica se já existe
		err(FATAL, SEMANTIC, "%s already defined in current scope\n", satrb->name);
		return -1;
	}
	symtab[symtab_next_entry] = *satrb;
	//memcpy(&symtab[symtab_next_entry], satrb, sizeof(satrb));
	debug("added to symtab[%d]: { name = %s, scope = %d, type = %d, ind = %d, attr = %d }\n",
			symtab_next_entry, symtab[symtab_next_entry].name, symtab[symtab_next_entry].scope,
			symtab[symtab_next_entry].type, symtab[symtab_next_entry].indirections, symtab[symtab_next_entry].attributes);
	return ++symtab_next_entry;
}

void symtab_asgm_params(int from, int to, SEMANTIC_ATTRIB *satbr) {
	debug("symtab_asgm_params\n");
	satbr->param = malloc((to - from) * sizeof(SEMANTIC_ATTRIB));
	int i;
	for(i = from, satbr->attributes = 0; i < to; i++, satbr->attributes++) {
		memcpy(&satbr->param[satbr->attributes], &symtab[i], sizeof(SEMANTIC_ATTRIB));
	}
}
