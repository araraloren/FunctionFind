/*
 * date：2015-8-7
 * author：blackcat
 * description:
 *
*/


#ifndef CLINFOREGISTRY_H
#define CLINFOREGISTRY_H

#include <clinfo.h>
#include <cltypedef.h>

#include <vector>
#include <string>

using std::string;
using std::vector;

//cpplogger namespace
CL_NAMESPACE_BEGIN

typedef void freeMemoryFunc(ClInfoPtr ptr);

void defaultFreeFunc( ClInfoPtr ptr);

class ClInfoRegistry
{
public:
    ~ClInfoRegistry();

	inline size_t size() const
    {
        return m_registry.size();
    }

    /*
     * 注册日志类型，info的内存必须是动态分配
     * 内存的释放由s_free函数来执行
     * 可以通过setFreeFunc来设置此函数
    */
	size_t registerInfo(const ClInfoPtr& info);

	ClInfoPtr operator[](size_t id);

	ClInfoPtr operator[](string name);

    /*
     * 通过日志id获取日志name
    */
	string getLogName(size_t id);

    /*
     * 通过日志name获取日志id
    */
	size_t getLogId(string name);

    /*
     * 遍历所有的日志类型，调用其内置的函数check
    */
    void checkInfo();

    inline std::vector<ClInfoPtr>::iterator begin()
    {
        return m_registry.begin();
    }

    inline std::vector<ClInfoPtr>::iterator end()
    {
        return m_registry.end();
    }


    static void setFreeFunc(freeMemoryFunc *value);

protected:
    static freeMemoryFunc* s_free;

protected:
    //日志类型数组
	std::vector<ClInfoPtr>  m_registry;
};

//cpplogger namespace
CL_NAMESPACE_END


#endif // CLINFOREGISTRY_H
