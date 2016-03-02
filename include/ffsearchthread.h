#ifndef FF_SEARCH_THREAD
#define FF_SEARCH_THREAD

#include <ffconfig.h>
#include <cctypedef.h>

NAMESPACE_FF_BEGIN



const size_t TASK_QUEUE_SIZE = 1024;

cc::thread_ret_t CC_THREAD search(cc::thread_para_t para);

NAMESPACE_FF_END

#endif
