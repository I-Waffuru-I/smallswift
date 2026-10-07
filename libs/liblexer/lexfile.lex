%{

  #include "libcomp/tokens.h"

  /* Keep track of current position of lex for error messages, i.e. 
   * the position just *after* the last token read */
  int line_nr = 1;
  int col_nr = 1; 

%}


digit [0-9]
letter [a-zA-Z]
letdig ({digit}|{letter})
identifier {letter}({letdig})*


  /* --- */
%%




\+ { return (PLUS); }
- { return (MINUS); }
\* { return (MUL); }
\/ { return (DIV); }
 
\< { return (LT); }
\> { return (GT); }
\<= { return (LE); }
\>= { return (GE); }
&& { return (AND); }
\|\| { return (OR); }
! { return (NOT); }
== { return (EQ); }
!= { return (NEQ); }
= { return (ASSIGN); }

; { return (SEMICOLON); }
\( { return (LPAREN); }
\) { return (RPAREN); }
\{ { return (LBRACE); }
\} { return (RBRACE); }
: { return (COLON); }
, { return (COMMA); }
-\> { return (ARROW); }

Int { return (INTTYPE); }
Bool { return (BOOLTYPE); }

true { return (TRUE); }
false { return (FALSE); }

func { return (FUNC); }
print { return (PRINT); }
if { return (IF); }
else { return (ELSE); }
while { return (WHILE); }
var { return (VAR); }
let { return (LET); }
return { return (RETURN); }
(\r\n|\r|\n) { return (NEWLINE); }

"//" { }

[ \t\v]+ {}

{digit}+ { return (INTEGER); }

{identifier} { return (IDENTIFIER); }


. {
    if (yytext[0] < ' '){ /* non-printable char */
      /*yyerror*/ fprintf(stderr,"illegal character: ^%c",yytext[0] + '@'); 
    }
    else {
      if (yytext[0] > '~') {
        /* non-printable char printed as octal int padded with zeros, eg \012*/
        /*yyerror(*/fprintf(stderr,"illegal character: \\%03o", (int) yytext[0]);
      }
      else {
        /*yyerror(*/fprintf(stderr,"illegal character: %s",yytext);
      }
    }
    /* lex read exactly one char; the illegal one */
    fprintf(stderr," at line %d column %d\n", line_nr, (col_nr-1));
}

  /* --- */
%%

/* Function called by (f)lex when EOF is read. If yywrap returns a
   true (non-zero) (f)lex will terminate and continue otherwise.*/
int yywrap(){
  return (1);
}
