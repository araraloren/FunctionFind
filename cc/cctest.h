#ifndef CCTEST_H
#define CCTEST_H 1

#include <ccconfig.h>
#include <cctypedef.h>

#include <vector>
#include <string>
#include <iostream>

ccNamespaceDeclare(cc)

template <typename T>
void
print(const std::vector<T>& vec, char sep = '\t')
{
    for (typename std::vector<T>::const_iterator ci = vec.begin();
            ci != vec.end();ci ++) {
        std::cerr << *ci <<sep;
    }
}

template <typename T>
void
println(const std::vector<T>& vec)
{
    for (typename std::vector<T>::const_iterator ci = vec.begin();
            ci != vec.end();ci ++) {
        std::cerr << *ci <<std::endl;
    }
}

ccNamespaceEnd(cc)

#endif
