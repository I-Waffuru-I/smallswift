#include <stdio.h>
#include "libcomp/tokens.h"
#include "liblexer/lexer.h"

#pragma GCC diagnostic ignored "-Wwrite-strings"

/* Array with tokens such that index = tokenid - 258 */
char *tokens[] = {
	"INTEGER",
	"IDENTIFIER",
	"PLUS",
	"MINUS",
	"MUL",
	"DIV",
	"LT",
	"GT",
	"LE",
	"GE",
	"AND",
	"OR",
	"NOT",
	"EQ",
	"NEQ",
	"VAR",
	"LET",
	"ASSIGN",
	"SEMICOLON",
	"NEWLINE",
	"LPAREN",
	"RPAREN",
	"LBRACE",
	"RBRACE",
	"PRINT",
	"TRUE",
	"FALSE",
	"IF",
	"ELSE",
	"WHILE",
	"COLON",
	"COMMA",
	"ARROW",
	"FUNC",
	"INTTYPE",
	"BOOLTYPE",
	"RETURN"
}; 

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("No file given.");
		return 1;
	}
	yyin = fopen(argv[1], "r");
	int tokenid;

	while ((tokenid = yylex())) {
		/* Token codes start from 258 */ 
		printf(" %s", tokens[tokenid-258]);

		/* Append value */
		if ( (tokenid == IDENTIFIER) || (tokenid == INTEGER) ) 
			printf("(\"%s\")", yytext);
	}
	printf("\n");
	return 0;
}
