find_package(FLEX REQUIRED)

FLEX_TARGET(lexfile
    lexfile.lex
    ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc
)

add_library(liblexer
    ${FLEX_lexfile_OUTPUTS}
)

target_include_directories(liblexer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(liblexer PUBLIC
    libcomp   # for libcomp/tokens.h
)
