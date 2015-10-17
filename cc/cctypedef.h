#ifndef CCTYPEDEF_H
#define CCTYPEDEF_H

#include <ccconfig.h>

#include <stdint.h> //for uint?_t and int?_t

#if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32)
#   include <pthread.h>
#elif (defined CC_PF_WIN32)
#	include <Windows.h>
#endif

/*
 * 如果cpp使用了c++11，那么使用标准的线程库以及原子变量库
 * if we used c++11, just use std::thread
*/
#if __cplusplus < 201103L
#   define nullptr NULL
#else
#   include <thread>
#endif

ccNamespaceDeclare(cc)

//about thread
#if (__cplusplus >= 201103L)
    ccTypeRegister(thread_t, std::thread);
    ccTypeRegister(thread_para_t, void*);
    ccTypeRegister(thread_ret_t, int);
#else
    #if (defined CC_PF_LINUX)
        ccTypeRegister(thread_t, pthread_t);
        ccTypeRegister(thread_para_t, void*);
        ccTypeRegister(thread_ret_t, void*);
    #elif (defined CC_PF_MINGW32)
        ccTypeRegister(thread_t, pthread_t);
        ccTypeRegister(thread_para_t, void*);
        ccTypeRegister(thread_ret_t, void*);
    #elif (defined CC_PF_WIN32)
        ccTypeRegister(thread_t, HANDLE);
        ccTypeRegister(thread_para_t, void*);
        ccTypeRegister(thread_ret_t, unsigned int);
    #endif
#endif

    typedef thread_ret_t CC_THREAD thread_func(thread_para_t);

//about number
using ::int8_t;
using ::int16_t;
using ::int32_t;
using ::int64_t;
using ::uint8_t;
using ::uint16_t;
using ::uint32_t;
using ::uint64_t;
using ::uintptr_t;

ccNamespaceEnd(cc)


#endif // CCTYPEDEF_H

