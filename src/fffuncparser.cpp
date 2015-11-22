#include <fffuncparser.h>

#ifdef FF_DEBUG
#   include <cstdio>
#endif

using namespace ff;

FuncParser::FuncParser(int argc, const char ** argv)
    :m_index(clang_createIndex(0, 0))
    ,m_tunit()
    ,m_argc(argc)
    ,m_argv(argv)
    ,m_func_list()
    ,m_file()
{

}

FuncParser::~FuncParser()
{
    clang_disposeIndex(this->m_index);
    clang_disposeTranslationUnit(this->m_tunit);
}

bool
FuncParser::parse(const std::string &file)
{
    if (file.empty()) {
        return false;
    }

    this->m_file = file;

    return this->startParse();
}

void FuncParser::unregisterKind(CXCursorKind kind)
{
    for (std::vector<CXCursorKind>::iterator it = this->m_kind.begin();
            it != this->m_kind.end();it ++) {
        if (*it == kind) {
            this->m_kind.erase(it);
            break;
        }
    }
}

bool
FuncParser::startParse()
{
    this->m_tunit = clang_parseTranslationUnit(this->m_index,
                                               this->m_file.c_str(),
                                               this->m_argv,
                                               this->m_argc,
                                               nullptr,
                                               0,
                                               CXTranslationUnit_None);

    CXCursor cursor = clang_getTranslationUnitCursor(this->m_tunit);

    int ret = clang_visitChildren(cursor, FuncParser::FP_vistor, CXClientData(this));

    return bool(ret == 0);
}

void
FuncParser::visitCursor(CXCursor cursor)
{
    CXCursorKind kind = clang_getCursorKind(cursor);

#ifndef FF_DEBUG
    if (this->filterKind(kind)) {
        return ;
    }
#endif

    CXString str = clang_getCursorKindSpelling(kind);

    FF_AVOID_WARNING(str);
    //add parser cpp or header file

#ifdef FF_DEBUG
    std::fprintf(stderr, "CursorKind:%s\n", clang_getCString(str));
#endif
}

CXChildVisitResult
FuncParser::FP_vistor(CXCursor cursor, CXCursor parent, CXClientData data)
{
    FF_AVOID_WARNING(parent);

    FuncParser* fp = static_cast<FuncParser*>(data);

    fp->visitCursor(cursor);

    return CXChildVisit_Continue;
}

bool
FuncParser::filterKind(CXCursorKind kind)
{
    for (std::vector<CXCursorKind>::iterator it = this->m_kind.begin();
            it != this->m_kind.end();it ++) {
        if (*it == kind) {
            return false;
        }
    }

    return true;
}


