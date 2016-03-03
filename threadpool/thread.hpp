#ifndef THREAD_H
#define THREAD_H

#include <ccthread.h>
#include <syncsemaphore.h>

template <typename Tp>
class Thread
{
public:
	explicit Thread(Tp* pool)
		:idle_(false)
		,quit_(false)
		,pool_(pool)
		,thread_()
        ,syncsem_()
	{}

	bool
	start(cc::thread_func func)
	{
		return this->thread_.start(func, this);
	}

	void
	detach()
	{
		this->thread_.detach();
	}

	void
	join()
	{
		this->thread_.join();
	}

	cc::thread_t*
	nativeThreadPtr()
	{
		return this->thread_.nativeThreadPtr();
	}

	Tp*
	currentPool() const
	{
		return this->pool_;
	}

	bool
	isIdle() const
	{
		return this->idle_;
	}

	void
	switchStatus()
	{
		this->idle_ = !this->idle_;
	}

    void
    initQuit()
    {
        syncsem_.init();
    }

	bool
    canQuit()
	{
        return this->quit_;
	}

	void
	setQuitFlag()
	{
		this->quit_ = true;
	}

    bool
    waitQuit()
    {
        return syncsem_.wait();
    }

    void
    quit()
    {
        syncsem_.notify();
    }
private:
    Thread(const Thread&);
    Thread& operator =(const Thread&);

private:
	bool idle_;

    volatile bool quit_;

	Tp* pool_;

	cc::Thread	thread_;

    SyncSemaphore syncsem_;
};

#endif // THREAD_H
