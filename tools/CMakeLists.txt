add_executable(lex_debug lex_debug.cpp)

target_link_libraries(lex_debug PRIVATE liblexer)
target_compile_options(lex_debug PRIVATE -Wall -Wextra)
