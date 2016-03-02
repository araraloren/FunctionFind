
#include <ffsearchtask.h>
#include <threadpool/threadpool.h>
#include <threadpool/thread.hpp>
#include <stdio.h>

cc::SSeqQueue<ITask*> g_completed;

void ff::DispatchTask::run()
{
    g_completed.init();

    const vector<CFile>& files = m_cmdopt->getFiles();

    for (vector<CFile>::const_iterator cit = files.begin(); \
         cit != files.end();cit ++) {
        ITask *task = new SearchTask(m_cmdopt, *cit);

        m_threadpool->dispatchTask(task);
    }

    printf("%s\n", "Run ok!");
}

void ff::SearchTask::run()
{
    g_completed.push(this);
}


