# mypas :: Compiladores UNESP @ RC /2013
Professor: Eraldo Pereira Marinho

Repositório disponível em: https://github.com/romajs/mypas

### Grupo 03 ~ Noturno
- André Cantelli  
- Daniel Felipe Lucas  
- José Romualdo dos Santos  
- Rodrigo Akio  
- Tulio Pereira  

## Sobre:

Um compilador pseudo-pascal escrito em C  

Limitaçõs da linguagem:
 - Não contempla ponteiros;
 - Não contempla tipos prórios;
 - Não suporta o uso de functions/procedures aninhados;
 - Arrays semelhantes as declarações me C (sem o ponto-ponto);

Algumas variações da gramática proposta pelo Eraldo:
 - Múltiplas declarações de IDs com um único VAR em 'vardeclr' e 'argspc'
 - Chamada de funciont/procedure com 'exprlist' opcional, apenas '()'

## Como utilizar:

Para compilar utilize o comando `make` e depois execute com `./mypas`
Para sair do programa utilize as teclas `ctrl+c`.  

O programa aceita duas diretivas (argumentos):  
- `-d` modo debug, todo os passos serão exibidos na saída padrão;
- `-t` modo de testes, executa diversos testes pré-definidos no início do programa;

## Sintax:

mypas -> PROGRAM ID ';' { specification } stmblk '.'

### Object declaration scope is defined next:

specification ->  vardeclr | sbrdeclr

vardeclr ->  VAR idlist ':' typespec ';' { idlist ':' typespec ';' }

sbrdeclr ->  sbrhead { vardeclr } stmblk ';'

idlist -> ID { ',' ID }

typespec -> smptype | ARRAY '[' UINT ']' OF typespec

smptype -> INTEGER | REAL | DOUBLE | BOOLEAN | STRING

sbrhead -> PROCEDURE ID argdef ';' | FUNCTION ID argdef ':' smptype ';'

argdef -> [ '(' arglist ')' ]

arglist -> argspc { ';' argspc }

argspc -> [ VAR ] idlist ':' smptype { idlist ':' smptype }

### Imperative scope is defined hereafter: 

stmt -> <epsilon> | ifstmt | whlstmt | forstmt | repstmt | idstmt | stmblk

stmblk -> BEGIN stmtlst END

stmtlst -> stmt { ';' stmt }

ifstmt -> IF expr THEN stmt [ ELSE stmt ]

whlstmt -> WHILE expr DO stmt

forstmt -> FOR ID indexing ':=' expr DOWNTO|TO expr DO stmt

repstmt -> REPEAT stmtlst UNTIL expr

idstmt -> ID [ parm | indexing ':=' expr ]

indexing -> {  '[' expr ']' }

parm -> '(' [ ')' | exprlst ')']

exprlst -> expr { ',' expr }

### Algebric and Boolean Expressions are defined hereafter: 

expr -> E [ relop E ]

relop -> '>' | '>=' | '<' | '<=' | '=' | '<>'

E -> [ '-' | NOT ] term { addop term }

addop -> '+' | '-' | OR

term -> fact { mulop fact }

mulop -> '' | '' | DIV | MOD | AND

fact -> ID [ param | indexing ]
