#ifndef CLANGTYPEWRAP_H
#define CLANGTYPEWRAP_H

#include <clang-c/Index.h>
#include <ffconfig.h>
#include <string>

NAMESPACE_FF_BEGIN

template <typename Tp, void (*Tf)(Tp)>
class RAIIHelper {
public:
    RAIIHelper()
    {  }

    RAIIHelper(Tp obj)
        :m_obj_ptr(obj)
    {  }

    RAIIHelper& operator = (const RAIIHelper&);

    RAIIHelper& operator = (Tp obj)
    {
        set(obj);

        return *this;
    }

    void set(Tp obj)
    {
        m_obj_ptr = obj;
    }

    Tp operator -> () const
    {
        return this->m_obj_ptr;
    }

    Tp get() const
    {
        return this->m_obj_ptr;
    }

    operator Tp ()
    {
        return get();
    }

    ~RAIIHelper()
    {
        Tf(m_obj_ptr);
    }

private:
    Tp  m_obj_ptr;
};

namespace clang {

typedef RAIIHelper<CXIndex, clang_disposeIndex>
    Index;

typedef RAIIHelper<CXTranslationUnit, clang_disposeTranslationUnit>
    TranslationUnit;

class String: public RAIIHelper<CXString, clang_disposeString> {
    std::string str() const
    {
        return std::string(clang_getCString(get()));
    }

    const char* cstr() const
    {
        return clang_getCString(get());
    }
};

}

NAMESPACE_FF_END

#endif // CLANGTYPEWRAP_H

