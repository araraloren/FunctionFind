project(FunctionFind)
cmake_minimum_required(VERSION 2.8)
set(SRC_LIST
    ./cpplogger/cllogger.cpp
    ./cpplogger/clinforegistry.cpp
    ./cpplogger/clinfo.cpp
    ./cc/ccpublic.cpp
    ./src/main.cpp
    ./src/ffoption.cpp
    ./src/fffuncparser.cpp
    ./threadpool/threadpool.cpp
    ./threadpool/syncsemaphore.cpp
    ./threadpool/thread.hpp
)
find_library(
    LIBCLANG
    clang
    /usr/lib/llvm/ /usr/lib64/llvm /usr/local/lib/llvm /usr/local/lib64/llvm
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
)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
