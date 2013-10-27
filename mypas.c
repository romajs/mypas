#include <mypas.h>

FILE *source, *object;

main(int argc, char *args[])
{
	source = stdin;
	object = stdout;

  match_args(argc, args);
  
	mypas();
}

void err(int type, int nature, const char *format, ...)
{
  char *message = "";
  switch(type) {
    case INFO:
      strcpy(message, "INFO");
      break;
    case WARN:
      strcpy(message, "WARNING");
      break;
    case FATAL:
      strcpy(message, "FATAL");
      break;
  }
  strcpy(message, "(");
  switch(type) {
    case LEXICAL:
      strcpy(message, "LEXICAL");
      break;
    case SYNTATIC:
      strcpy(message, "SYNTATIC");
      break;
    case SEMANTIC:
      strcpy(message, "SEMANTIC");
      break;
  }
  strcpy(message, "): ");
  strcpy(message, format);
	va_list arg;
	va_start(arg, format);
	vfprintf(object, message, arg);
	va_end(arg);
  exit(0);
}