#ifndef _FFSEARCHTASK_H_
#define _FFSEARCHTASK_H_

#include <ffconfig.h>
#include <threadpool/itask.h>
#include <ffoption.h>

class ThreadPool;
class CommandOption;

NAMESPACE_FF_BEGIN

class DispatchTask: public ITask
{
public:
    DispatchTask(const CommandOption* cmdopt, ThreadPool* threadpool)
        :m_cmdopt(cmdopt)
        ,m_threadpool(threadpool)
    {}

    virtual ~DispatchTask()
    { }

    void run();

private:
    const CommandOption* m_cmdopt;
    ThreadPool* m_threadpool;
};

class SearchTask: public ITask
{
public:
    SearchTask(const CommandOption* cmdopt, const CFile& file, ThreadPool* threadpool = nullptr)
        :m_cmdopt(cmdopt)
        ,m_threadpool(threadpool)
        ,m_file(file)
    {}

    virtual ~SearchTask()
    { }

    void run();

private:
    const CommandOption* m_cmdopt;
    ThreadPool* m_threadpool;
    CFile m_file;
};


NAMESPACE_FF_END

#endif
