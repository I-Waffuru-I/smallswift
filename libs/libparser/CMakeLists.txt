find_package(BISON REQUIRED)


# build parser.tab.cc
BISON_TARGET(parser
    parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.hh
    COMPILE_FLAGS "-Wcounterexamples"
)

add_library(libparser
    ${BISON_parser_OUTPUTS}
)

# parser.tab.hh includen
target_include_directories(libparser PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)


target_link_libraries(libparser PUBLIC
    # Expr types and stuff 
    libcomp
    # voor lexer.h en yylex()
    liblexer
)
