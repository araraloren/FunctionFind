
#include <syncsemaphore.h>
#include <semaphore.h>
#include <cassert>

bool
SyncSemaphore::init(int max)
{
	int ret = 0;

	ret = ! ::sem_init(&sync_sem_, 0, max);
	assert( ret );

	return ret;
}

bool SyncSemaphore::wait()
{
	int ret = 0;

	ret = ! ::sem_wait(&sync_sem_);
	assert( ret );

	return ret;
}


bool SyncSemaphore::notify()
{
	int ret = 0;

	ret = ! ::sem_post(&sync_sem_);
	assert( ret );

	return ret;
}
