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
	char message[255];	
	sprintf(message, "Line %d [", lineno);
	switch(type) {
		case INFO:
			strcat(message, "INFO");
			break;
		case WARN:
			strcat(message, "WARNING");
			break;
		case FATAL:
			strcat(message, "FATAL");
			break;
		default:
			strcat(message, "UNKNOW");
			break;
	}
	strcat(message, "/");
	switch(nature) {
		case LEXICAL:
			strcat(message, "LEXICAL");
			break;
		case SYNTATIC:
			strcat(message, "SYNTATIC");
			break;
		case SEMANTIC:
			strcat(message, "SEMANTIC");
			break;
		default:
			strcat(message, "OTHER");
			break;
	}
	strcat(message, "]: ");
	strcat(message, format);
	va_list arg;
	va_start(arg, format);
	vfprintf(object, message, arg);
	va_end(arg);
	exit(0);
}