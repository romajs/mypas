#include <debug.h>

FILE *out;

void set_debug(FILE *f) {
	out = f;
}

void debug(const char *format, ...) {
	if(!out) return;
	va_list arg;
	va_start(arg, format);
	vfprintf(out, format, arg);
	va_end(arg);
}

void debug_symtab(const char table[MAX_SYMTAB_ENTRIES][MAX_ID_SIZE], const double *value, int size) {
	if(!out) return;
	debug("SYMTAB(size = %d):\n", size);
	int i;
	debug("| I | ADDR \t| VALUE \t| LEXEME \t|\n");
	for(i = 0; i < size; i++) {
		debug("| %d | %d \t| %.2f \t| %s \t| \n", i, table[i],
		value[i], table[i]);
	}
}

void debug_oper(int v[MAX_RECURSION_SIZE][MAX_STACK_SIZE], int i_size, int j_size) {
	if(!out) return;
	debug("OPER[%d][%d]:\n", i_size, j_size);
	int i, j;
	for(i = 0; i <= i_size; i++) {
		debug(" %d: |", i);
		for(j = 0; j <= j_size; j++) {
			debug(" %c ", v[i][j]);
	}
		debug("|\n");
	}
}