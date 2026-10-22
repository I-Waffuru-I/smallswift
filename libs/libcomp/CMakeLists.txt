add_library(libcomp
    src/libcomp.cpp
    src/parsevals.cpp
    src/typecheck.cpp
)

target_include_directories(libcomp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(libcomp PRIVATE -Wall -Wextra)
