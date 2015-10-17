#ifndef THREAD_H
#define THREAD_H

#include <ccthread.h>

template <typename Tp>
class Thread
{
public:
	explicit Thread(Tp* pool)
		:idle_(false)
		,quit_(false)
		,pool_(pool)
		,thread_()
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

private:
	bool idle_;

	bool quit_;

	Tp* pool_;

	cc::Thread	thread_;
};

#endif // THREAD_H
