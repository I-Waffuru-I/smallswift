#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "liblexer/lexer.h"
#include "libcomp/libcomp.h"
#include "libcomp/parsevals.h"
#include "parser.tab.hh"

int main(int argc, char* argv[]) {
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
   if (parse_res == 0) {
      std::cout << std::endl << "parse successful" << std::endl;
   } else {
      exit(parse_res);
   }

   if (result) {
      std::cout << "debugging parsed tree:" << std::endl;
      result->debug();
   } else {
      std::cout << "result is empty" << std::endl;
   }
   return parse_res;
}
