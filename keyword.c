#include <string.h>
#include <keyword.h>

keyword iskeyword(char const *query)
{
	keyword k;
	for(k = BEGIN; k < END + 1; k++) {
		if(strcmp(query, keywords_lexeme[k - _]) == 0)
      return k;
  }
	return 0;
}