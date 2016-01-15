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

class SignatureParser {
public:
    static Signature parse(const string& str);

private:
    static Signature parse1(const string& str);

    static Signature parse2(const string& str, string::size_type pos);

    static ArgList parseArgument(const string& str, string::size_type l, string::size_type r);
};

NAMESPACE_FF_END

#endif // FFPARSER_H
