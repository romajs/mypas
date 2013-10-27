#ifndef KEYWORD_H
#define KEYWORD_H

#define FOREACH_ENUM(ENUM) \
	ENUM(BEGIN = 0x10000) \
	ENUM(PROGRAM) \
	ENUM(VAR) \
	ENUM(ATTR) \
  ENUM(EQ) \
  ENUM(NEQ) \
  ENUM(GRT) \
  ENUM(GEQ) \
  ENUM(LSR) \
  ENUM(LEQ) \
	ENUM(AND) \
	ENUM(OR) \
	ENUM(DIV) \
	ENUM(MOD) \
	ENUM(NOT) \
	ENUM(ARRAY) \
	ENUM(OF) \
	ENUM(INTEGER) \
	ENUM(BOOLEAN) \
	ENUM(REAL) \
	ENUM(DOUBLE) \
	ENUM(STRING) \
	ENUM(IF) \
	ENUM(THEN) \
	ENUM(ELSE) \
	ENUM(FUNCTION) \
	ENUM(PROCEDURE) \
	ENUM(REPEAT) \
	ENUM(UNTIL) \
	ENUM(FOR) \
	ENUM(TO) \
	ENUM(DOWNTO) \
	ENUM(DO) \
	ENUM(WHILE) \
	ENUM(END) \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum keywords {
  FOREACH_ENUM(GENERATE_ENUM)
};

static const char *keywords_lexeme[] = {
  FOREACH_ENUM(GENERATE_STRING)
};

#endif // KEYWORD_H

typedef enum keywords keyword;

keyword iskeyword(char const *query);