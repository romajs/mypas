#ifndef TOKENS_H
#define TOKENS_H

// semantic types for expressions
enum {
	ID = 1025,
	INT_CTE,		// integer
	REAL_CTE,	// real
	DBL_CTE,		// double
	BOOL_CTE,	// boolean
	STR_CTE		// string
};

// error types
enum errors {
	INFO = 0x100,
	WARN,
	FATAL,
	OTHER
};

// nature error types
enum natures {
	LEXICAL = 0x100,
	SYNTATIC,
	SEMANTIC,
};

#define   		MAX_ID_SIZE              		32	// Tamanho máximo dos identificadores
#define			MAX_IND_SIZE					0x100 // número máximo de indireções
#define			MAX_ARGS_SIZE					0x100 // número máximo de argumentos

#define   		MAX_INT_DIG             		10	// Número máximo de dígitos de um inteiro
#define	  		MAX_PRC_DIG		          		17	// Número máximo de dígitos de precisão de ponto flutuante
#define			MAX_EXP_DIG							 3 // número máximo de dígitos no expoente

#define	  		MAX_STR_LEN		          	0x100	// Número máximo de caracteres de uma string

#define	 		MAX_STACK_SIZE		   		0x100	// Tamanho máximo das pilhas			
#define			MAX_RECURSION_SIZE   		0x100	// Número máximo de recursões	

#define	  		MAX_SYMTAB_ENTRIES			0x100	// Tamanho máximo de symtab				
#define	  		MAX_MEM_SIZE		     		0x100	// Dúvida: Eraldo colocou isso ? mas não é ligada a SYMTAB?			

#endif // TOKENS_H
