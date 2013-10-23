enum{
	BEGIN=0x10000,
	ATTR,
	NOT,
	OR,
	AND,
	DIV,
	MOD,
	ARRAY,
	OF,
	BOOLEAN,
	DOUBLE,
	ELSE,
	FUNCTION,
	IF,
	INTEGER,
	PROCEDURE,
	PROGRAM,
	REAL,
	REPEAT,
	STRING,
	THEN,
	UNTIL,
	VAR,
	WHILE,
	DO,
	FOR,
	TO,
	DOWNTO,
	END
};

int iskeyword(char const *query);