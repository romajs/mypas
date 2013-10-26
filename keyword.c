#include <string.h>
#include <keyword.h>

char const *keyword_lexemes[]={
	"BEGIN",
	"ATTR",
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

keyword iskeyword(char const *query)
{
	keyword k;
	for(k = BEGIN; k < END + 1; k++)
		if(strcmp(query, keyword_lexemes[k - BEGIN]) == 0)
      return k;
	return 0;
}