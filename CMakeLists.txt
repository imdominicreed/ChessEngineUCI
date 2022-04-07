cmake_minimum_required(VERSION 3.0.0)
project(chess VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
enable_testing()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -flto")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

file(GLOB src_files CONFIGURE_DEPENDS
		"ChessBoardAPI/src/*")

add_executable(dom_eng tpalphabeta.cpp talphabeta.cpp tt.cpp tt.hpp UCI.cpp palphabeta.cpp alphabeta.cpp eval.cpp eval.hpp minmax.cpp pminmax.cpp engine.hpp ${src_files})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(dom_eng PRIVATE Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
