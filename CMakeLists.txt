cmake_minimum_required(VERSION 3.0.0)
project(chess VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
enable_testing()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -flto")

file(GLOB src_files CONFIGURE_DEPENDS
		"ChessBoardAPI/src/*")

add_executable(dom_eng UCI.cpp eval.cpp eval.hpp minmax.cpp minmax.hpp ${src_files})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
