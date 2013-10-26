#include <mypas.h>

FILE *source, *object;

main(int argc, char *args[]) {

	source = stdin;
	object = stdout;

  match_args(argc, args);
  
	mypas();
}