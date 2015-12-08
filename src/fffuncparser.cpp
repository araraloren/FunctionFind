#include <fffuncparser.h>

#ifdef FF_DEBUG
#   include <cstdio>
#endif

using namespace ff;

FuncParser::FuncParser(int argc, const char ** argv)
    :m_index(clang_createIndex(0, 0))
    ,m_tu()
    ,m_argc(argc)
    ,m_argv(argv)
    ,m_func_list()
    ,m_file()
{
    this->registerKind(CXCursor_FunctionDecl);
    this->registerKind(CXCursor_FunctionTemplate);
    this->registerKind(CXCursor_CXXMethod);
    this->registerKind(CXCursor_Constructor);
    this->registerKind(CXCursor_Destructor);
    this->registerKind(CXCursor_ConversionFunction);
    this->registerKind(CXCursor_ClassDecl);
    this->registerKind(CXCursor_StructDecl);
}

FuncParser::~FuncParser()
{

}

bool
FuncParser::parse(const std::string &file)
{
    if (file.empty()) {
        return false;
    }

    m_file = file;

    return this->startParse();
}

void FuncParser::unregisterKind(CXCursorKind kind)
{
    m_kind.erase(kind);
}

bool
FuncParser::startParse()
{
    m_tu = clang_parseTranslationUnit(
            m_index,
            m_file.c_str(),
            m_argv,
            m_argc,
            nullptr,
            0,
            CXTranslationUnit_None
    );

    CXCursor cursor = clang_getTranslationUnitCursor(m_tu);

    int ret = clang_visitChildren(cursor, FuncParser::FP_vistor, CXClientData(this));

    return bool(ret == 0);
}

CXChildVisitResult
FuncParser::visitCursor(CXCursor cursor)
{
    CXCursorKind kind = clang_getCursorKind(cursor);

    if (this->filterKind(kind)) {
        return CXChildVisit_Continue;
    }

    switch(kind) {
        case CXCursor_FunctionDecl: { }
        case CXCursor_FunctionTemplate: { }
        case CXCursor_CXXMethod: { }
        case CXCursor_Constructor: { }
        case CXCursor_Destructor: { }
        case CXCursor_ConversionFunction: { }
        case CXCursor_ClassDecl: { }
        case CXCursor_StructDecl: { }
        default:break;
    }

    return CXChildVisit_Continue;
}

CXChildVisitResult
FuncParser::FP_vistor(CXCursor cursor, CXCursor parent, CXClientData data)
{
    FF_AVOID_WARNING(parent);

    FuncParser* fp = static_cast<FuncParser*>(data);

    return fp->visitCursor(cursor);
}

bool
FuncParser::filterKind(CXCursorKind kind)
{
    return m_kind.count(kind) > 0;
}

