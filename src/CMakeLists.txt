add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
	libcomp 
	libparser
	liblexer 
)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)


