#ifndef SYNCSEMAPHORE_H
#define SYNCSEMAPHORE_H

#include <semaphore.h>//posix semaphore
#include <cassert>

class SyncSemaphore
{
public:
    SyncSemaphore()
    {}

    ~SyncSemaphore()
    {
        int ret = 0;

        ret = ! ::sem_destroy(&sync_sem_);
        assert( ret );
    }

    bool init(int initvalue = 0);

	bool wait();

	bool notify();

    bool full();

    int getValue();

private:
    SyncSemaphore(const SyncSemaphore&);
    SyncSemaphore& operator= (const SyncSemaphore&);

protected:
	sem_t   sync_sem_;
    int     max_;
};

#endif // SYNCSEMAPHORE_H
