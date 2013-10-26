/*@<symtab.c>*/
#include <string.h>

#include <symtab.h>

SEMANTIC_ATTRIB symtab[MAX_SYMTAB_ENTRIES+1];

int symtab_next_entry = 1;

symtab_lookup(char const *query)
{
	int i;
	for(i=symtab_next_entry-1;i>0;i--){
		if(strcmp(symtab[i].name,query)==0)return i;
	}
	return 0;
}

symtab_add_list(int n, char const *symlist[], int type, int offsettype)
{
  int i;
  for(i=0;i<n;i++){
    int entry = symtab_lookup(symlist[i]);
    if(entry && symtab[entry].offsettype>=offsettype){
	    fprintf(object, "%s already defined in current scope\n",symlist[i]);
	    return -1;
    }
    strcpy(symtab[symtab_next_entry].name, symlist[i]);
    symtab[symtab_next_entry].type = type;
    symtab[symtab_next_entry].offsettype = offsettype;
    symtab_next_entry++;
  }
  return symtab_next_entry;
}