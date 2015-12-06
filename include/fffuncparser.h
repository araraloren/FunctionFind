#ifndef FUNCPARSER_H
#define FUNCPARSER_H


#include <ffconfig.h>
#include <fffunction.h>
#include <clang-c/Index.h>
#include <set>
#include <string>
#include <ffraii.h>

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
        this->m_kind.insert(kind);
    }

    void
    unregisterKind()
    {
        std::set<CXCursorKind> x_;

        this->m_kind.swap(x_);
    }

    void
    unregisterKind(CXCursorKind kind);

private:
    //add CXCursorKind parser
    template <CXCursorKind kind> Function
    parseCursor(CXCursor cursor);

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
    clang::Index m_index;
    clang::TranslationUnit m_tu;

    int m_argc;
    const char** m_argv;

    std::vector<Function> m_func_list;
    std::string m_file;
    std::set<CXCursorKind> m_kind;
};

template <> Function
FuncParser::parseCursor<CXCursor_FunctionDecl>(CXCursor cursor)
{
    FF_AVOID_WARNING(cursor);

    return Function();
}

template <> Function
FuncParser::parseCursor<CXCursor_FunctionTemplate>(CXCursor cursor)
{
    FF_AVOID_WARNING(cursor);

    return Function();
}

template <> Function
FuncParser::parseCursor<CXCursor_CXXMethod>(CXCursor cursor)
{
    FF_AVOID_WARNING(cursor);

    return Function();
}

template <> Function
FuncParser::parseCursor<CXCursor_Constructor>(CXCursor cursor)
{
    FF_AVOID_WARNING(cursor);

    return Function();
}

template <> Function
FuncParser::parseCursor<CXCursor_Destructor>(CXCursor cursor)
{
    FF_AVOID_WARNING(cursor);

    return Function();
}

template <> Function
FuncParser::parseCursor<CXCursor_ConversionFunction>(CXCursor cursor)
{
    FF_AVOID_WARNING(cursor);

    return Function();
}

template <> Function
FuncParser::parseCursor<CXCursor_ClassDecl>(CXCursor cursor)
{
    FF_AVOID_WARNING(cursor);

    return Function();
}

template <> Function
FuncParser::parseCursor<CXCursor_StructDecl>(CXCursor cursor)
{
    FF_AVOID_WARNING(cursor);

    return Function();
}


NAMESPACE_FF_END

#endif // FUNCPARSER_H
