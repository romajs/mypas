PROGRAM test101;
VAR
  id: Integer;
VAR
  produto: String;
VAR
  valor, custo: Real;
VAR
  qtde: Double;
VAR
  ativo: Boolean;
PROCEDURE SayHello;
var
  msg: String;
BEGIN
  msg := 'Hello';
  Writeln(msg);
END;
FUNCTION Fibonacci(x: Integer): Boolean;
BEGIN
  if (x = 0 or x = 1) then
    result := x
  else
    result := Fibonnaci(x - 2) + Fibonacci(x - 1);
END;
BEGIN
END.