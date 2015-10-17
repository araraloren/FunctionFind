#ifndef SYNCSEMAPHORE_H
#define SYNCSEMAPHORE_H

#include <semaphore.h>//posix semaphore


class SyncSemaphore
{
public:
	bool init(int max);

	bool wait();

	bool notify();

protected:
	sem_t   sync_sem_;
};

#endif // SYNCSEMAPHORE_H
