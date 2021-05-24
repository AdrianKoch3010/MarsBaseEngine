%%
[a-zA-Z_][a-zA-Z0-9_]* 		return IDENTIFIER;

[+-]?[0-9]+					return INT;
[0-9]+[uU]					return UNSIGNED_INT;
[+-]?[0-9]+[lL]				return LONG;
[0-9]+[uU][lL]				return UNSIGNED_LONG;
[+-]?[0-9]*"."[0-9]+		return DOUBLE;
[+-]?[0-9]*"."[0-9]+[fF]?	return FLOAT;
[+-]?[0-9]+"."[0-9]*		return DOUBLE;
[+-]?[0-9]+"."[0-9]*[fF]?	return FLOAT;

["](.|[^"])*["]		        return STRING;

#.*                         return COMMENT;

[\=]						return ASSIGN;
[;]							return END_ASSIGN;
[ \n\t\r]+	             	// Ignore whitespace
.							return ERROR;
