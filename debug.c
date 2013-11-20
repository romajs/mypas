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