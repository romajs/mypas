#include <mypas.h>

FILE *source, *object;

main(int argc, char *args[]) {

	source = stdin;
	object = stdout;

	// clear parser before start
	lookahead = EOF;
	lexeme[0] = 0;
	fflush(source);

  // call gramar initial symbol
	lookahead = gettoken(source);
	mypas();
}