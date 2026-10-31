%code requires {
    #include "libcomp/libcomp.h"
    #include "libcomp/parsevals.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <string>
#include "libcomp/libcomp.h"
#include "liblexer/lexer.h"

int yylex();
void yyerror(const char *s);

%}

%locations

%define parse.error detailed 

%code {
    using namespace libcomp;
}

%union {
    int ival;
    char* sval;
    libcomp::Exp exp;
    libcomp::StmList* stml;
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
%token DO         295

%left PLUS MINUS
%left MUL DIV
%left AND OR
%nonassoc LT GT LE GE EQ NEQ   /* "1 < 2 < 3" voorkomen */

%right UMINUS

%type <ival> INTEGER
%type <sval> IDENTIFIER
%type <exp> boollit valexpr valueexpr
%type <exp> boolexpr 
%type <stml> program stmts
%type <stm> line
%type <stm> declarestm assignstm
%type <stm> block ifstm



/* ---- */
%%

program:
       stmts { result = $$; }




block:
     LBRACE stmts RBRACE { $$ = new BlockStm($2); }

stmts:
      /* leeg programma */   { $$ = new StmList(); }
    | stmts line {
        if ($2 != nullptr) $1->add($2);
        $$ = $1;
    }
    ;


line:
    endline { $$ = nullptr; }
    | valueexpr endline { $$ = new ExpStm($1); }
    | declarestm endline
    | assignstm endline 
    | DO block endline { $$ = new DoBlockStm($2); }
    | ifstm endline 
    ;
endline:
    NEWLINE 
    | SEMICOLON
    ;

ifstm: 
    IF valueexpr block { $$ = new IfStm($2, $3); } 
    | IF valueexpr block ELSE block { $$ = new IfStm($2, $3, $5); } 
    | IF valueexpr block ELSE ifstm { $$ = new IfStm($2, $3, $5); } 
    ;

declarestm:
    LET IDENTIFIER ASSIGN valueexpr { $$ = new DeclareStm($2, $4, false); free($2); }
    | VAR IDENTIFIER ASSIGN valueexpr { $$ = new DeclareStm($2, $4, true); free($2); }
    ;

assignstm:
    IDENTIFIER ASSIGN valueexpr { $$ = new AssignStm($1, $3); free($1); }
    ;

valueexpr:
     valexpr
     | boolexpr
     ;

valexpr:
      INTEGER { $$ = new NumExp($1); }
    | IDENTIFIER { $$ = new VariableExp($1); free($1); }
    | MINUS INTEGER %prec UMINUS { $$ = new NumExp(-$2); }
    | valexpr PLUS valexpr { $$ = new OpExp($1, Plus, $3); }
    | valexpr MINUS valexpr { $$ = new OpExp($1, Minus, $3); }
    | valexpr MUL valexpr { $$ = new OpExp($1, Multiply, $3); }
    | valexpr DIV valexpr { $$ = new OpExp($1, Divide, $3); }
    | LPAREN valexpr RPAREN { $$ = new ParenExp($2); }
    ;

boollit:
    TRUE   { $$ = new TFExp(true);  }
    | FALSE   { $$ = new TFExp(false); }
    ;

boolexpr:
    boollit
    | boolexpr AND boolexpr { $$ = new BoolExp($1, And, $3); }
    | boolexpr OR boolexpr { $$ = new BoolExp($1, Or, $3); }
    | valexpr GT valexpr { $$ = new BoolExp($1, Gt, $3); }
    | valexpr LT valexpr { $$ = new BoolExp($1, Lt, $3); }
    | valexpr GE valexpr { $$ = new BoolExp($1, Ge, $3); }
    | valexpr LE valexpr { $$ = new BoolExp($1, Le, $3); }
    | valexpr EQ valexpr { $$ = new BoolExp($1, Eq, $3); }
    | valexpr NEQ valexpr { $$ = new BoolExp($1, Neq, $3); }
    | LPAREN boolexpr RPAREN { $$ = new ParenExp($2); }
    ;



/*  ---- */
%%

void yyerror(const char *s) {
    // fprintf(stderr, "parse error: %s\n", s);
    fprintf(stderr,"Parse error | line: %d, %d\n%s\n",yylloc.first_line, yylloc.first_column, s);
    exit(1);
}
