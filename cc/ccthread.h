#ifndef CCTHREAD_H
#define CCTHREAD_H

#include <ccconfig.h>
#include <cctypedef.h>

ccNamespaceDeclare(cc)

class Thread
{
public:
	Thread()
		:thread_()
	{}

	bool
	start(cc::thread_func func, cc::thread_para_t para)
	{
#if __cplusplus >= 201103L
		cc::thread_t thr(func, para);

		thr.swap(thread_);
#else
#   if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32)
			int ret = pthread_create(&this->thread_, 0, func, para);

			if(ret != 0){
				return false;
			}
#   elif (defined CC_PF_WIN32)
		thread_ = (cc::thread_t)_beginthreadex(0, 0, func, para, 0, 0);
#   endif
#endif

		return true;
	}

	void
	join()
	{
#if __cplusplus >= 201103L
		this->thread_.join();
#else
#   if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32)
		pthread_join(this->thread_, 0);
#	elif (defined CC_PF_WIN32)
		static const int S_WAIT_FOR = 5000;

		if (WAIT_TIMEOUT == WaitForSingleObject(this->thread_, S_WAIT_FOR)){
			TerminateThread(this->thread_, 1);
		}
#   endif
#endif
	}

	void
	detach()
	{
#if __cplusplus >= 201103L
		this->thread_.join();
#else
#   if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32)
		pthread_join(this->thread_, 0);
#	elif (defined CC_PF_WIN32)
#warning "win32 thread can not detach"
#   endif
#endif
	}

	cc::thread_t*
	nativeThreadPtr()
	{
		return &this->thread_;
	}

private:
	cc::thread_t thread_;
};

ccNamespaceEnd(cc)

#endif // CCTHREAD_H

