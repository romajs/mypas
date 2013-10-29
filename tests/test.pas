PROGRAM test101;
VAR
  id: Integer;
  produto: String;
  valor, custo: Real;
  qtde: Double;
  ativo: Boolean;
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
BEGIN
END.