#include <header/codegen.h>

void assembly(const char *format, ...) {
	if(!object) return;
	va_list arg;
	va_start(arg, format);
	vfprintf(object, format, arg);
	va_end(arg);
}

void datalbl() {
	assembly(".data\n");
}

void globlbl() {
	assembly(".globl\tmain\n");
}

void mainlbl() {
	assembly("main:\n");
}

void setlbl(int label) {
	assembly(".L%d\n", label);
}

void gofalse(int label) {	
	assembly("\tjz L%d\n", label);
}

void gotolbl(int label) {	
	assembly("\tjmp .L%d\n", label);
}

void gotosteplbl(int step, int label) {	
	if(step > 0) {
		jg(label);
	} else {
		jl(label);
	}
}

void pop(int offset) {
	assembly("\tpop %%eax, %d\n", offset);
}

void mov(int value) {
	assembly("\tmov %d, %%eax\n", value);
}

void cmp(const char* varadress) {
	assembly("\tcmpl %s, %%eax\n", varadress);
}

void jg(int value) {
	assembly("\tjg .L%d\n", value);
}

void jl(int value) {
	assembly("\tjl .L%d\n", value);
}

void stepc(int step, const char* varadress) {
	if(step > 0) {
		inc(varadress);
	} else {
		dec(varadress);
	}
}

void inc(const char* varadress) {
	assembly("\tincl %s\n", varadress);
}

void dec(const char* varadress) {
	assembly("\tdecl %s\n", varadress);
}