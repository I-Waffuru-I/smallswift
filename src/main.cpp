#include "libcomp/cstd.h"
#include "libcomp/libcomp.h"
#include "libcomp/parsevals.h"
#include "libcomp/typecheck.h"
#include "liblexer/lexer.h"
#include "parser.tab.hh"
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char **argv) {
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
    std::cout << std::endl << "PARSE SUCCESSFUL: " << std::endl;
  } else {
    exit(parse_res);
  }

  result->debug(0);
  std::cout << std::endl << std::endl;

  typecheck::Scope program;
  cstd::loadStd(program);
  result->check(program);

  std::cout << std::endl << "RUNTIME:" << std::endl;

  std::shared_ptr<interp::Context> root = std::make_shared<interp::Context>();
  cstd::loadStd(*root);
  result->interp(*root);

  // std::cout << std::endl << "ANALYSIS:" << std::endl;
  // typecheck::str b = "b";
  // std::cout << root->lookup(b)->asInt() << std::endl;
  // typecheck::str c = "c";
  // std::cout << root->lookup(c)->asInt() << std::endl;
  //
  return 0;
}
