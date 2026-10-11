%code requires {
    #include "libcomp/libcomp.h"
}

%{

#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include "libcomp/libcomp.h"
#include "liblexer/lexer.h"

int yylex();
void yyerror(const char *s);

%}

%code {
    using namespace libcomp;
}

%union {
    int ival;
    libcomp::Exp exp;
    libcomp::Expl expl;
    libcomp::Stm stm;
}

/* zou nice zijn om da gwn te importen van tokens.h ma idk of da gaat */
%token <exp> INTEGER    258
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

%type <exp> intexpr
%type <exp> line 
%type <expl> program

/* ---- */
%%

program:
      /* leeg programma */   { $$ = nullptr; result = $$; }
    | program line  {
        if ($2 != nullptr){ /* not empty line */
            if ($1 == nullptr) {
                $$ = new EndExp($2);
            } else {
                $$ = new LineExp($2, $1);
                result = $$;
            };
        }
    }
    ;

line:
      NEWLINE { $$ = nullptr; }
    | intexpr NEWLINE { $$ = $1; }
    ;

intexpr:
      INTEGER { $$ = new NumExp(yylval.ival); }
    | intexpr PLUS intexpr { $$ = new OpExp($1, Plus, $3); }
    | intexpr MINUS intexpr { $$ = new OpExp($1, Minus, $3); }
    | intexpr MUL intexpr { $$ = new OpExp($1, Multiply, $3); }
    | intexpr DIV intexpr { $$ = new OpExp($1, Divide, $3); }
    | LPAREN intexpr RPAREN { $$ = new ParenExp($2); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "parse error: %s\n", s);
    exit(1);
}
