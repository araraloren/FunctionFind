#ifndef ITASK_H
#define ITASK_H

class ITask {
public:
    virtual ~ITask() {}

    virtual void run() = 0;
};

#endif // ITASK_H

