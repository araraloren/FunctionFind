#include <iostream>
#include <cpplogger/cllogger.h>
#include <ccsseqqueue.h>
#include <cctest.h>
#include <functional>
#include <ffoption.h>
#include <ffsearchthread.h>

using namespace std;

int main(int argc, char** argv)
{
    ff::CommandOption *ffop = ff::CommandOption::getInstance();

    ffop->parseArgv(argc, argv);
    ffop->debugOption();

    ff::search(nullptr);

    return 0;
}

