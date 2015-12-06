project(FunctionFind)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -g -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -Os -Wall -Wextra -Werror")
set(SRC_LIST
    ./cpplogger/cllogger.cpp
    ./cpplogger/clinforegistry.cpp
    ./cpplogger/clinfo.cpp
    ./cc/ccpublic.cpp
    ./cc/cctest.cpp
    ./src/main.cpp
    ./src/ffoption.cpp
    ./src/fffuncparser.cpp
    ./threadpool/threadpool.cpp
    ./threadpool/syncsemaphore.cpp
    ./threadpool/thread.hpp
    ./threadpool/itask.h
)
exec_program(llvm-config . ARGS "--libdir" OUTPUT_VARIABLE LLVMLD)
exec_program(llvm-config . ARGS "--includedir" OUTPUT_VARIABLE LLVMIN)
find_library(
    LIBCLANG
    clang
    ${LLVMLD}
)
set(LIBRARIES
    pthread
    ${LIBCLANG}
)
include_directories(
    ./
    ./include/
    ./cc/
    ./cpplogger/
    ./threadpool/
    ${LLVMIN}
)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
