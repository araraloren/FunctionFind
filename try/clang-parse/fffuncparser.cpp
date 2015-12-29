#include <cstdio>
#include <fffuncparser.h>
#include <stdarg.h>
#include <cstring>

static void
note(const char* format, ...)
{
    va_list list;
    va_start(list, format);
     
    vfprintf(stdout, format, list);
     
    va_end(list);
}

FuncParser::FuncParser(int argc, char ** argv)
    :m_index(clang_createIndex(0, 0))
    ,m_tunit()
    ,m_argc(argc)
    ,m_argv(argv)
    ,m_file()
{

}

FuncParser::~FuncParser()
{
	if (!this->m_file.empty()) {
		clang_disposeIndex(this->m_index);
		clang_disposeTranslationUnit(this->m_tunit);
	}
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
    note("Start parse file %s \n", m_file.c_str());

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

CXChildVisitResult
FuncParser::visitCursor(CXCursor cursor)
{
	CXFile file;
	unsigned int row, col, off;

	CXSourceLocation loc = clang_getCursorLocation(cursor);

	clang_getFileLocation(loc, &file, &row, &col, &off);

	const char* fname = clang_getCString(clang_getFileName(file));
	
	if (std::strcmp(fname, m_file.c_str())) {
		//note("%s <-> %s\n", fname, m_file.c_str());
		return CXChildVisit_Continue;
	}

	//note(" -> +++\n");
	
    CXCursorKind kind = clang_getCursorKind(cursor);

    CXString str = clang_getCursorKindSpelling(kind); 

    note("-> CursorKind:%s + ", clang_getCString(str));

	CXString spell = clang_getCursorSpelling(cursor);

	note(" -> CursorSpelling:%s + ", clang_getCString(spell));

	CXString crts = clang_getTypeSpelling(clang_getCursorResultType(cursor));

	note(" -> CursorResultType:%s ", clang_getCString(crts));

	CXString cts = clang_getTypeSpelling(clang_getCursorType(cursor));

	note(" -> CursorType:%s \n", clang_getCString(cts));

	clang_disposeString(str);
	clang_disposeString(spell);
	clang_disposeString(cts);
	clang_disposeString(crts);

	int n = clang_Cursor_getNumArguments(cursor);

	note(" -> {%d} [ ", n);

	for (int i = 0;i < n; i++) {
		CXCursor argc = clang_Cursor_getArgument(cursor, i);

		CXCursorKind ckind = clang_getCursorKind(argc);

    	CXString ckind_str = clang_getCursorKindSpelling(ckind); 

    	note("-> %s |  ", clang_getCString(ckind_str));

		CXString ckind_spell = clang_getCursorSpelling(argc);

		note("-> %s | ", clang_getCString(ckind_spell));

		CXString ckind_crts = clang_getTypeSpelling(clang_getCursorResultType(argc));

		note("-> %s | ", clang_getCString(ckind_crts));

		CXString ckind_cts = clang_getTypeSpelling(clang_getCursorType(argc));

		note("-> %s  ++  ", clang_getCString(ckind_cts));
	}

	note(" ] \n");

	CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);
	CXSourceRange range  = clang_getCursorExtent(cursor);

	CXToken* token;
	unsigned int num;

	clang_tokenize(tu, range, &token, &num);

	if (num > 0) {
		for (unsigned int i = 0;i < num - 1;i ++) {
			CXString ts = clang_getTokenSpelling(tu, token[i]);

			CXTokenKind tk = clang_getTokenKind(token[i]);

			note("\tToken ^_^ < Spelling:%s Kind:%d > ", clang_getCString(ts), tk);

			clang_disposeString(ts);

			CXSourceLocation sl = clang_getTokenLocation(tu, token[i]);

			clang_getFileLocation(sl, &file, &row, &col, &off);

			note("row[%u] col[%u] off[%u]\n", row, col, off);
		}
	}

	return CXChildVisit_Continue;
}

CXChildVisitResult
FuncParser::FP_vistor(CXCursor cursor, CXCursor parent, CXClientData data)
{
    FF_AVOID_WARNING(parent);
	FF_AVOID_WARNING(cursor);

    FuncParser* fp = static_cast<FuncParser*>(data);

    fp->visitCursor(cursor);
	//if (parent.kind != 1) {
	//	fp->visitCursor(parent);
	//}

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


