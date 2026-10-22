add_executable(lex_debug lex_debug.cpp)

target_link_libraries(lex_debug PRIVATE liblexer)
target_compile_options(lex_debug PRIVATE -Wall -Wextra)


add_executable(parse_debug parse_debug.cpp)

target_link_libraries(parse_debug PRIVATE libparser liblexer)
target_compile_options(parse_debug PRIVATE -Wall -Wextra)
