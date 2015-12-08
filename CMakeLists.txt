project(FunctionFind)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -g -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -Os -Wall -Wextra -Werror")
set(SRC_LIST
    ./cc/ccpublic.h
    ./cc/ccatomic.h
    ./cc/cctypedef.h
    ./cc/cctest.h
    ./cc/ccthread.h
    ./cc/ccpublic.cpp
    ./cc/ccconfig.h
    ./cc/ccsseqqueue.h
    ./cc/cctest.cpp
    ./cc/ccseqqueue.h

    ./cpplogger/cllogger.h
    ./cpplogger/cltypedef.h
    ./cpplogger/cllogger.cpp
    ./cpplogger/clinforegistry.cpp
    ./cpplogger/clconfig.h
    ./cpplogger/clinforegistry.h
    ./cpplogger/clinfo.h
    ./cpplogger/clinfo.cpp

    ./include/fffunction.h
    ./include/ffpublic.h
    ./include/ffoption.h
    ./include/ffconfig.h
    ./include/ffraii.h
    ./include/fffuncparser.h

    ./src/main.cpp
    ./src/fffuncparser.cpp
    ./src/ffoption.cpp

    ./threadpool/syncsemaphore.h
    ./threadpool/threadpool.cpp
    ./threadpool/threadpool.h
    ./threadpool/syncsemaphore.cpp
    ./threadpool/itask.h
    ./threadpool/thread.hpp
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
