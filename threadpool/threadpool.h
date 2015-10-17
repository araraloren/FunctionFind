#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <ccconfig.h>
#include <cctypedef.h>
#include <ccsseqqueue.h>
#include <syncsemaphore.h>
#include <vector>

#include <thread.hpp>

class ITask;

class ThreadPool
{
public:
	explicit ThreadPool(size_t pool_size, size_t task_queue_size);

	~ThreadPool();

	bool
	init();

	bool
	dispatchTask(ITask* task);

	void
	stop();

	inline size_t
	size()
	{
		return this->size_;
	}

	void
	notifyAllThreadQuit();

protected:
	static cc::thread_ret_t s_run(cc::thread_para_t arg);

	void
	notifyPoolQuited();

	void
	waitForTask()
	{
		this->sync_sem_.wait();
	}

	void
	popTask(ITask*& task)
	{
		this->task_queue_.pop(task);
	}

	void
	release();

protected:
	cc::Atomic<size_t> quit_;

	cc::SSeqQueue<ITask*>	task_queue_;

	cc::Atomic<size_t> task_count_;

	const size_t size_;

	std::vector<Thread<ThreadPool>*> thread_pool_;

	SyncSemaphore sync_sem_;
};

#endif // THREADPOOL_H
