#include <cstdio>
#include "liblexer/lexer.h"
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

    int result = yyparse();
    if (result == 0) {
        printf("parse successful\n");
    }
    return result;
}
