#include <string.h>
#include <keyword.h>

char const *keyword[]={
	"BEGIN",
	"NOT",
	"OR",
	"AND",
	"DIV",
	"MOD",
	"ARRAY",
	"OF",
	"BOOLEAN",
	"DOUBLE",
	"ELSE",
	"FUNCTION",
	"IF",
	"INTEGER",
	"PROCEDURE",
	"PROGRAM",
	"REAL",
	"REPEAT",
	"STRING",
	"THEN",
	"UNTIL",
	"VAR",
	"WHILE",
	"DO",
	"FOR",
	"TO",
	"DOWNTO",
	"END",
};

iskeyword(char const *query)
{
	int i;
	for(i=BEGIN;i<END+1;i++)
		if(strcmp(query,keyword[i-BEGIN])==0)return 1;
	return 0;
}

