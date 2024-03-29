cmake_minimum_required(VERSION 3.0.0)
project(chess VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
enable_testing()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -flto")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer")

file(GLOB board CONFIGURE_DEPENDS
	"ChessBoardAPI/board/*")

file(GLOB move CONFIGURE_DEPENDS
	"ChessBoardAPI/move/*")

file(GLOB magic CONFIGURE_DEPENDS
	"ChessBoardAPI/magic/*")

file(GLOB util CONFIGURE_DEPENDS
	"ChessBoardAPI/util/*")

file(GLOB engines CONFIGURE_DEPENDS "engines/*")

include_directories("engines" "board" "magic" "util")

file(GLOB eval CONFIGURE_DEPENDS "eval/*")

file(GLOB nnue CONFIGURE_DEPENDS "eval/nnue/*")

add_executable(uci main.cpp ${eval} ${engines} ${board} ${move} ${magic} ${util} ${nnue})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(uci PRIVATE Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
