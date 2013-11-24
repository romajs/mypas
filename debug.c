#include <debug.h>

FILE *out;

void set_debug(int flag) {  
  if(flag) {
    out = stdout;
  } else {
    out = NULL;
  }
}

void debug(const char *format, ...) {
	if(!out) return;
	va_list arg;
	va_start(arg, format);
	vfprintf(out, format, arg);
	va_end(arg);
}

void debug_symtab_entry(int i)
{
  debug("symtab[%d]: { name = %s, scope = %d, type = %d, objtype = %d", i, symtab[i].name,
    symtab[i].scope, symtab[i].type, symtab[i].objtype);
  int j;
  if(symtab[i].indirections) {
    debug(", dimensions = %d ", symtab[i].indirections);
    for(j = 1; j < symtab[i].indirections; j++) {
      debug("x %d ", symtab[i].dimension[j]);
    }
  }
  if(symtab[i].attributes) {
    debug(", arguments = %d }", symtab[i].attributes);
    for(j = 0; j < symtab[i].attributes; j++) {
      debug("\n\t[%d]: { name = %s, scope = %d, type = %d, ind = %d }",
        j, symtab[i].argument[j].name, symtab[i].argument[j].scope,
        symtab[i].argument[j].type, symtab[i].argument[j].indirections);
    }      
  }
  debug(" } \n");
}

void debug_symtab(void) {
	debug("<debug_symtab: entries = %d>\n", symtab_next_entry - 1);
  int i;
  for(i = symtab_next_entry - 1; i > 0; i--){
    debug_symtab_entry(i);
  }
  debug("</debug_symtab>\n");
}
