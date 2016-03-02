#include <threadpool.h>
#include <itask.h>

ThreadPool::ThreadPool(size_t pool_size, size_t task_queue_size)
	:quit_(0)
	,task_queue_(task_queue_size)
	,task_count_(0)
	,size_(pool_size)
	,thread_pool_(pool_size, nullptr)
	,sync_sem_()
{

}

ThreadPool::~ThreadPool()
{
	this->release();
}

bool
ThreadPool::init()
{
    task_queue_.init();

	if (!sync_sem_.init(task_queue_.size())) {
		return false;
	}

	for(size_t i = 0;i < size_;++ i) {
		Thread<ThreadPool>* thr = new Thread<ThreadPool>(this);

		if (!thr->start(ThreadPool::s_run)) {
			this->release();
			return false;
		}

        this->thread_pool_[i] = thr;
	}

	return true;
}

bool
ThreadPool::dispatchTask(ITask* task)
{
    if (!task && this->sync_sem_.full()) {
		return false;
	}

	this->task_queue_.push(task);

    this->sync_sem_.notify();

	return true;
}

void
ThreadPool::stop()
{
	this->notifyAllThreadQuit();

	for (size_t i = 0;i < this->size_;i ++) {
		this->sync_sem_.notify();
	}

	while(this->quit_.get() < this->size_) {
		cc::microSleep(500);
	}
}

void ThreadPool::notifyAllThreadQuit()
{
	for(std::vector<Thread<ThreadPool>*>::iterator it = thread_pool_.begin();    \
					it != thread_pool_.end();it ++) {
			Thread<ThreadPool>* thread = (*it);

			thread->setQuitFlag();
			thread->detach();
	}
}

cc::thread_ret_t
ThreadPool::s_run(cc::thread_para_t arg)
{
	Thread<ThreadPool>* thread = static_cast<Thread<ThreadPool>*>(arg);

	ThreadPool* current_pool = thread->currentPool();

	ITask* task = nullptr;

    while(true) {
        //wait for task
		current_pool->waitForTask();

        //thread shoud be quit
		if (thread->canQuit()) {
			current_pool->notifyPoolQuited();
			break;
		}

        //switch thead status
		thread->switchStatus();

        //pop task
		current_pool->popTask(task);

        //run task
        if (task){
            task->run();
            task = nullptr;
        }

        //switch thead status
		thread->switchStatus();
	}

	return cc::thread_ret_t(0);
}

void
ThreadPool::notifyPoolQuited()
{
	this->quit_.inc();
}

void
ThreadPool::release()
{
	for(std::vector<Thread<ThreadPool>*>::iterator it = thread_pool_.begin();    \
					it != thread_pool_.end();it ++) {
			delete (*it);
			*it = nullptr;
	}
}
