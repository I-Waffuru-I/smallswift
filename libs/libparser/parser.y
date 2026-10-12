%code requires {
    #include "libcomp/libcomp.h"
    #include "libcomp/parsevals.h"
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
%token INTEGER    258
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
%left AND OR
%nonassoc LT GT LE GE EQ NEQ   /* "1 < 2 < 3" voorkomen */

%type <ival> INTEGER
%type <exp> boollit
%type <exp> intexpr
%type <exp> boolexpr 
%type <exp> valueexpr
%type <exp> line 
%type <expl> program

/* ---- */
%%

program:
      /* leeg programma */   { $$ = nullptr; result = $$; }
    | program line {
        if ($2 == nullptr) {
            $$ = $1;
        } else if ($1 == nullptr) {
            $$ = new EndExp($2);
            result = $$;
        } else {
            $$ = new LineExp($2, $1);
            result = $$;
        }
    }
    ;

line:
    endline { $$ = nullptr; }
    | valueexpr endline { $1->debug();printf(" as line\n"); $$ = $1; }
    ;

endline:
    NEWLINE 
    | SEMICOLON
    ;


valueexpr:
     intexpr
     | boolexpr
     ;

intexpr:
      INTEGER { $$ = new NumExp($1); }
    | intexpr PLUS intexpr { $$ = new OpExp($1, Plus, $3); $$->debug(); printf("\n"); }
    | intexpr MINUS intexpr { $$ = new OpExp($1, Minus, $3); $$->debug(); printf("\n"); }
    | intexpr MUL intexpr { $$ = new OpExp($1, Multiply, $3); $$->debug(); printf("\n"); }
    | intexpr DIV intexpr { $$ = new OpExp($1, Divide, $3); $$->debug(); printf("\n"); }
    | LPAREN intexpr RPAREN { $2->debug(); printf(" in paren\n"); ; $$ = new ParenExp($2); }
    ;

boollit:
    TRUE   { $$ = new TFExp(true);  }
    | FALSE   { $$ = new TFExp(false); }
    ;

boolexpr:
    boollit
    | boolexpr AND boolexpr { $$ = new BoolExp($1, And, $3); }
    | boolexpr OR boolexpr { $$ = new BoolExp($1, Or, $3); }
    | intexpr GT intexpr { $$ = new BoolExp($1, Gt, $3); }
    | intexpr LT intexpr { $$ = new BoolExp($1, Lt, $3); }
    | intexpr GE intexpr { $$ = new BoolExp($1, Ge, $3); }
    | intexpr LE intexpr { $$ = new BoolExp($1, Le, $3); }
    | intexpr EQ intexpr { $$ = new BoolExp($1, Eq, $3); }
    | intexpr NEQ intexpr { $$ = new BoolExp($1, Neq, $3); }
    | LPAREN boolexpr RPAREN { $2->debug(); printf(" in paren\n"); ; $$ = new ParenExp($2); }
    ;



/*  ---- */
%%

void yyerror(const char *s) {
    fprintf(stderr, "parse error: %s\n", s);
    exit(1);
}
