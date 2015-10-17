#include <iostream>
#include <cpplogger/cllogger.h>
#include <ffoption.h>
#include <ccsseqqueue.h>

using namespace std;

int main(int argc, char** argv)
{
    ff::FFOption *ffop = ff::FFOption::getInstance();

    ffop->parseCommandArgs(argc, argv);

    ffop->debugPrintStatus();

    cc::SSeqQueue<int> squeue;

    squeue.init();

    squeue.push(1);

    cout <<FF_TO_SRTING(squeue)<<" size -> "<<squeue.count()<<endl;

    int x = 0;

    squeue.pop(x);

    cout <<"get x -> "<<x<<endl;
    cout <<FF_TO_SRTING(squeue)<<" size -> "<<squeue.count()<<endl;

    return 0;
}

