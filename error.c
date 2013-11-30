#include <header/error.h>

/* função para exibição de erros, (não é tratamento) interrompe a execução 
 * caso o erro seja do tipo 'FATAL'
*/
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
  if(type == FATAL) {
    fprintf(stdout, "Compilation failed.\n");
    exit(0);
  }
}