PROGRAM test101;
VAR
  id: Integer;
  produto: String;
  valor, custo: Real;
  qtde: Double;
  ativo: Boolean;
  alai: Array[3] of integer;
VAR
	matrix: Array[3] of array [4] of integer;
{ Função que imprime "Hello" na saída }
PROCEDURE SayHello;
var
  msg: String;
BEGIN
  msg := 'Hello';
  Writeln(msg);
END;
{ Função de Fibonacci }
FUNCTION Fibonacci(x: Integer): Integer;
BEGIN
  IF (x = 0) or (x = 1) THEN
    result := x
  ELSE
    result := Fibonacci(x - 2) + Fibonacci(x - 1);
END;
{ Função de Fatorial }
FUNCTION Fatorial(i: Integer): Integer;
VAR
	x: Integer;
BEGIN
	x := 1;
	WHILE(i > 0) DO
	BEGIN
		x := x * i;
		i := i - 1;
	END;	
	result := x;
END;	
{ Loop infinito }
PROCEDURE Infinito;
BEGIN
  REPEAT
    { to the infinty and beyond... }
    ;
  UNTIL true
END;
{ Imprime elementos de um Array }
PROCEDURE ImprimeArray;
VAR
  arr: ARRAY[10] OF Integer;
  i: Integer;
BEGIN
  FOR i := 0 TO 10 DO
    WriteLn(arr[i]);
END;
PROCEDURE testerelop(x, y: Integer; s: String);
BEGIN
  IF (z > y) THEN
		s := 'ok'
  ELSE IF (z >= y) THEN
		s := 'ok'
  ELSE IF (z < y) THEN
		s := 'ok'
  ELSE IF (z <= y) THEN
		s := 'ok'
  ELSE IF (z >= y) THEN
		s := 'ok'
  ELSE IF (z <> y) THEN
		s := 'ok'		
  ELSE
		s := 'not ok';
END;
BEGIN
	SayHello();
	SayHello;
	Fibonacci(5);
	Fibonacci(6);
	valor := qtde * custo;
	testerelop(-1287930123.1234546789, 100.5626179281093e-308); { ashdgausgduasgdugbahisdas}
	nome := 'abacaxi';
END.