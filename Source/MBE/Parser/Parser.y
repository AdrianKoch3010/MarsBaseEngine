%scanner		Scanner.h
%scanner-token-function d_scanner.lex()
%token IDENTIFIER INT UNSIGNED_INT LONG UNSIGNED_LONG DOUBLE FLOAT STRING COMMENT ASSIGN
%%
startrule:
	  startrule line
	| line
	;
	
line:
	  statement EOS
	| COMMENT
	;
	
statement:
	  IDENTIFIER ASSIGN value
	;
	
value:
	  INT
	| UNSIGNED_INT
	| LONG
	| UNSIGNED_LONG
	| DOUBLE
	| FLOAT
	| STRING
	;