#ifndef FFPARSER_H
#define FFPARSER_H

#include <ctime>
#include <vector>
#include <string>
#include <cstring>
#include <ffconfig.h>
#include <ccpublic.h>
#include <fffunction.h>

using std::string;

NAMESPACE_FF_BEGIN

class FunctionParser {
public:
    static Signature parse(const string& function);

private:

};

NAMESPACE_FF_END

#endif // FFPARSER_H
