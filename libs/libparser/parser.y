%{

#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include "liblexer/lexer.h"


int yylex();
void yyerror(const char *s);

%}

%union {
    int ival;
}

/* zou nice zijn om da gwn te importen van tokens.h ma idk of da gaat */
%token <ival> INTEGER    258
%token IDENTIFIER 259
%token PLUS       260
%token MINUS      261
%token MUL        262
%token DIV        263
%token LT         264
%token GT         265
%token LE         266
%token GE         267
%token AND        268
%token OR         269
%token NOT        270
%token EQ         271
%token NEQ        272
%token VAR        273
%token LET        274
%token ASSIGN     275
%token SEMICOLON  276
%token NEWLINE    277
%token LPAREN     278
%token RPAREN     279
%token LBRACE     280
%token RBRACE     281
%token PRINT      282
%token TRUE       283
%token FALSE      284
%token IF         285
%token ELSE       286
%token WHILE      287
%token COLON      288
%token COMMA      289
%token ARROW      290
%token FUNC       291
%token INTTYPE    292
%token BOOLTYPE   293
%token RETURN     294

%left PLUS MINUS
%left MUL DIV

%type <ival> intexpr

/* ---- */
%%

program:
    | program line 
    ;

line:
      NEWLINE
    | intexpr NEWLINE { printf("line: %i\n", $1); }
    ;

intexpr:
      INTEGER { printf("i %i\n", yylval.ival); $$ = yylval.ival; }
    | intexpr PLUS intexpr { $$ = $1 + $3; }
    | intexpr MINUS intexpr { $$ = $1 - $3; }
    | intexpr MUL intexpr { $$ = $1 * $3; }
    | intexpr DIV intexpr { $$ = $1 / $3; }
    | LPAREN intexpr RPAREN { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "parse error: %s\n", s);
    exit(1);
}
