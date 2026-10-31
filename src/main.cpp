#include <iostream>
#include <string>
#include "liblexer/lexer.h"
#include "parser.tab.hh"
#include "libcomp/parsevals.h"
#include "libcomp/typecheck.h"

int main(int argc, char** argv) {
    std::cout << "hey" << std::endl;

    std::string input = "";

   if (argc < 2) {
      printf("No file given.\n");
      return 1;
   }

   yyin = fopen(argv[1], "r");
   if (!yyin) {
      std::cout << "Could not open file:" << argv[1] << std::endl;
      return 1;
   }

   int parse_res = yyparse();
   if (parse_res == 0 && result) {
      std::cout << std::endl << "parse successful" << std::endl;
   } else {
      exit(parse_res);
   }

   result->debug(0);
   std::cout << std::endl << std::endl;


   typecheck::Scope globalscope;
   typecheck::registerStdFuncs(globalscope);
   typecheck::Scope program = typecheck::Scope(globalscope);
   result->check(program);

   return 0;
}
