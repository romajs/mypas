/*@<symtab.h>*/
#include <stdio.h>

#include <tokens.h>

struct _semantic_attrib_ {
	char name[MAX_ID_SIZE];
	int scope;
	int offset;
	int type;
	int offsettype;
	int indirections;
	int objtype;
	int dimension[MAX_IND_SIZE];
	//SEMANTIC_ATTRIB argattrb[MAX_ARGS_SIZE];
};

typedef struct _semantic_attrib_ SEMANTIC_ATTRIB;

extern SEMANTIC_ATTRIB symtab[MAX_SYMTAB_ENTRIES+1];

extern int symtab_next_entry;

int symtab_lookup(char const *);

int symtab_add_list(int, char const *[], int, int);

extern FILE *source;

extern FILE *object;