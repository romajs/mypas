/*@<symtab.h>*/
#include <stdio.h>

#include <tokens.h>

typedef struct _semantic_atrib_ {
	char name[MAX_ID_SIZE];
	int scope;
	int type;
	int offset;
	
	//int offsettype; ???
	int objtype; // ATOMIC | SUBROUTINE | POINTER
		
	int dimension[MAX_IND_SIZE]; 
	int indirections; // quantidade de indireções da lista de dimensões
	
	struct _semantic_atrib_ *argument;
	int attributes; // quantidade de atributos na lista de argumentos
	
} SEMANTIC_ATTRIB;

extern SEMANTIC_ATTRIB symtab[MAX_SYMTAB_ENTRIES+1];

extern int symtab_next_entry;

int symtab_lookup(const char *, int);

int symtab_retrieve(const char *, int);

int symtab_validate(const char*);

int symtab_add_list(int, char [MAX_SYMTAB_ENTRIES][MAX_ID_SIZE + 1], int, int, int, int[MAX_IND_SIZE]);

void set_subroutine_argument_list(int, int, int);

extern FILE *source;

extern FILE *object;