#ifndef FUNCPARSER_H
#define FUNCPARSER_H


#include <ffconfig.h>
#include <fffunction.h>
#include <clang-c/Index.h>
#include <vector>
#include <string>

#if __cplusplus < 201103L
#   define nullptr NULL
#endif

NAMESPACE_FF_BEGIN

class FuncParser
{
public:
    FuncParser(int argc, const char** argv);
    ~FuncParser();

    bool
    parse(const std::string& file);

    Function
    match(const FunctionSignature& fs);

    void
    registerKind(CXCursorKind kind)
    {
        this->m_kind.push_back(kind);
    }

    void
    unregisterKind()
    {
        std::vector<CXCursorKind> x_;

        this->m_kind.swap(x_);
    }

    void
    unregisterKind(CXCursorKind kind);

private:
    bool
    startParse();

    void
    visitCursor(CXCursor cursor);

private:
    static CXChildVisitResult
    FP_vistor(CXCursor cursor, CXCursor parent, CXClientData data);

    bool
    filterKind(CXCursorKind kind);

private:
    CXIndex m_index;
    CXTranslationUnit m_tunit;

    int m_argc;
    const char** m_argv;

    std::vector<Function> m_func_list;
    std::string m_file;
    std::vector<CXCursorKind> m_kind;
};

NAMESPACE_FF_END

#endif // FUNCPARSER_H
