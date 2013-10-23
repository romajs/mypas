#include <mypas.h>

FILE* source, destination;

main(int argc, char *args[]) {

	source = stdin;
	//destination = stdout;

	// clear parser before start
	lookahead = EOF;
	lexeme[0] = 0;
	fflush(source); // não adianta muito, mas...

	lookahead = gettoken(source);
	mypas();
}