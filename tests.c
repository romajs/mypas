#include <tests.h>

int total = 0; // total de testes executados
int done = 0; // total de testes sucedidos

test(const char* filename)
{  
  /*int status;
  pid_t child;
  
  // inicia novo processo, para não "sujar" as pilhas, etc...
  if(!(child = fork())) {*/
    fprintf(object, "Testing: \"%s\"\n", filename);
    source = fopen(filename, "r+");
    mypas();
    fclose(source);
  /*}
  else { // aguardo o término do filho
    waitpid(child, &status, 0);
  }*/
}

// função que descreve os teste a serem executados
void tests(void)
{
	fprintf(object, "INITIALIZING TESTS...\n");
  
  int i;
  glob_t glob_data;

  // busca na pasta "tests" por todos os arquivos ".pas"
  switch(glob("tests/*.pas", 0, NULL, &glob_data) ) {
    case GLOB_NOSPACE:
      fprintf(object, "Glob: Out of memory\n");
      return;
    case GLOB_ABORTED:
      fprintf(object, "Glob: Reading error\n");
      return;
    case GLOB_NOMATCH:
      fprintf(object, "Glob: No files found\n");
      return;
    default:
      break;
  }

  // executa cada teste individualmente para cara arquivo encontrado
  for(i = 0; i < glob_data.gl_pathc; i++) {
    test(glob_data.gl_pathv[i]);
    // TODO: check test status
    total++;
  }
    
  // necessário limpar o buffer do glob "glob_data"
  globfree(&glob_data);
  symtab_lookup("");

	// cálculo dos totais
  fprintf(object, "Total: %d\n", total);
  fprintf(object, "Done: %d\n", done);
  fprintf(object, "Failed: %d\n", total - done);
  fprintf(object, "Average: %.2f %%\n", (double) done / total * 100);
  
  fprintf(object, "ALL TESTS EXECUTED!\n");
  source = stdin;
}