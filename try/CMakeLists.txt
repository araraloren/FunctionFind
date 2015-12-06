project(fftry)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -g -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -Os -Wall -Wextra -Werror")
set(SRC_LIST
    fffuncparser.cpp
    fffuncparser.h
    main.cpp
)
find_library(
    LIBCLANG
    clang
    /usr/lib/llvm/ /usr/lib64/llvm /usr/local/lib/llvm /usr/local/lib64/llvm
)
set(LIBRARIES
    ${LIBCLANG}
)
include_directories(
    ./
)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
