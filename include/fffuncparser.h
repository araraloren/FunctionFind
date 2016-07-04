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
    match(const Signature& fs);

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
    template <CXCursorKind>
    Function
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
    Function func;

    clang::String fname(clang_getCursorSpelling(cursor));

    func.func_name = fname.str();

    int n = clang_Cursor_getNumArguments(cursor);

    if (n < 0) {
        func.arg_list = ArgList();
    } else if (n == 0) {
        func.arg_list = ArgList(Type::T_Void);
    }

    ArgList arglist(Type::T_List);

    arglist.count = n;
    for (int i = 0;i < n; i++) {
        CXCursor arg = clang_Cursor_getArgument(cursor, i);

        clang::String argname = clang_getCursorSpelling(arg);

        clang::String argtypename = clang_getTypeSpelling(clang_getCursorType(arg));

        arglist.arglist.push_back(ArgumentType(argtypename.str(), argname.str()));
    }

    return func;
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
