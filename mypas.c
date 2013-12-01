#include <header/mypas.h>

main(int argc, char *argv[])
{
	source = stdin;
	object = stdout;
  
  if(argc > 1) {
    int i;
    /* parse main arguments */
    for(i = 1; i < argc; i++) {
      source = fopen(argv[i], "r+");
      /* call gramar initial symbol */
      lookahead = gettoken(source);    
      mypas();      
      fprintf(stdout, "Compilation successful for file \"%s\".\n", argv[i]);
      fclose(source);
    }  
  } else {  
    /* call gramar initial symbol */
    lookahead = gettoken(source);    
    mypas();  
    fprintf(stdout, "Compilation successful.\n");
  }
}