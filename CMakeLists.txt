cmake_minimum_required(VERSION 3.0.0)
project(chess VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
enable_testing()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -flto")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=thread")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=thread")

file(GLOB board_api CONFIGURE_DEPENDS
	"ChessBoardAPI/src/*")

file(GLOB engines CONFIGURE_DEPENDS "engines/*")

include_directories("engines")

file(GLOB eval CONFIGURE_DEPENDS "eval/*")

add_executable(uci main.cpp ${eval} ${engines} ${board_api})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(uci PRIVATE Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
