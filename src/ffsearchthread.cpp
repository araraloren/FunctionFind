
#include <ffsearchthread.h>
#include <ffoption.h>
#include <threadpool/threadpool.h>
#include <ffsearchtask.h>

cc::thread_ret_t ff::search(cc::thread_para_t para)
{
    FF_AVOID_WARNING(para);

    CommandOption* cmdopt = CommandOption::getInstance();
    ThreadPool  threadpool(cmdopt->getJobThread(), TASK_QUEUE_SIZE);
    DispatchTask* task = new DispatchTask(cmdopt, &threadpool);

    threadpool.init();

    threadpool.dispatchTask(task);

    cc::microSleep(10);

    threadpool.stop();

    delete task;

    return cc::thread_ret_t(0);
}
