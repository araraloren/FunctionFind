#ifndef FUNCPARSER_H
#define FUNCPARSER_H

#include <clang-c/Index.h>
#include <vector>
#include <string>

#if __cplusplus < 201103L
#   define nullptr NULL
#endif

#define FF_AVOID_WARNING(x) (void)x

class FuncParser
{
public:
    FuncParser(int argc, char** argv);
    ~FuncParser();

    bool
    parse(const std::string& file);
	
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

    CXChildVisitResult
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
    char** m_argv;

    std::string m_file;
    std::vector<CXCursorKind> m_kind;
};

#endif // FUNCPARSER_H
