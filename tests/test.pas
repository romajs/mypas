PROGRAM test101;
VAR
  id: Integer;
  produto: String;
  valor, custo: Real;
  qtde: Double;
  ativo: Boolean;
{ Fun��o que imprime "Hello" na sa�da }
PROCEDURE SayHello;
var
  msg: String;
BEGIN
  msg := 'Hello';
  Writeln(msg);
END;
{ Fun��o de Fibonacci }
FUNCTION Fibonacci(x: Integer): Integer;
BEGIN
  IF (x = 0) or (x = 1) THEN
    result := x
  ELSE
    result := Fibonacci(x - 2) + Fibonacci(x - 1);
END;
{ Fun��o de Fatorial }
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
BEGIN
END.