#include <mypas.h>

main(int argc, char *args[])
{
	source = stdin;
	object = stdout;  
  
	/* call gramar initial symbol */
	lookahead = gettoken(source);
  
	mypas();
}