#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "liblexer/lexer.h"
#include "libcomp/libcomp.h"
#include "parser.tab.hh"

int main(int argc, char* argv[]) {
   if (argc < 2) {
      printf("No file given.\n");
      return 1;
   }

   yyin = fopen(argv[1], "r");
   if (!yyin) {
      printf("Could not open file: %s\n", argv[1]);
      return 1;
   }

   int parse_res = yyparse();
   if (parse_res == 0) {
      printf("parse successful\n");
   } else {
      exit(parse_res);
   }

   if (libcomp::result) {
      libcomp::result->debug();
   } else {
      std::cout << "result is empty" << std::endl;
   }
   return parse_res;
}
