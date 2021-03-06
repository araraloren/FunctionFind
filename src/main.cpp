#include <iostream>
#include <cpplogger/cllogger.h>
#include <ccsseqqueue.h>
#include <cctest.h>
#include <functional>
#include <ffoption.h>

using namespace std;

int main(int argc, char** argv)
{
    ff::CommandOption *ffop = ff::CommandOption::getInstance();

    ffop->parseArgv(argc, argv);

    ffop->debugOption();

    cc::SSeqQueue<int> squeue;

    squeue.init();

    squeue.push(1);

    //cout <<FF_TO_SRTING(squeue)<<" size -> "<<squeue.count()<<endl;

    int x = 0;

    squeue.pop(x);

    cout <<"get x -> "<<x<<endl;
    //cout <<FF_TO_SRTING(squeue)<<" size -> "<<squeue.count()<<endl;

#if __cplusplus >= 201103L
    cc::println(cc::splitString(string("1 2  4 dksja    ida  qwd www"), string("\\s+")));
#else
    cc::println(cc::splitString(string("1 2  4 dksja    ida  qwd www")));
#endif

    cout <<cc::trim(string("   ; "))<<endl;

    return 0;
}

