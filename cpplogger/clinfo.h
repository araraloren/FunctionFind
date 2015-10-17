
/*
 * 日期：2015-8-6
 * 作者：blackcat
 * 描述：该文件是cpplogger的日志信息类型定义
 *
*/

/*
 * 修订记录
*/

/*
 * 日期：2015-8-6
 * 修订内容：
*/

#ifndef CLINFO_H
#define CLINFO_H

#include <clconfig.h>
#include <cltypedef.h>

#include <string>
#include <fstream>
#include <ctime>

using std::ofstream;
using std::string;

//cpplogger namespace
CL_NAMESPACE_BEGIN

/*
 * 日志的运作模式
*/
enum log_mode_t
{
    LM_LOOP_TIME = 1,   //循环 一定时间生成一个文件
    LM_LOOP_SIZE,       //循环 一定大小生成一个文件
    LM_LOOP,
    LM_ONCE_TIME,       //一次性 一定时间只生成一个文件
    LM_ONCE_SIZE,       //一次性 一定大小只生成一个文件
};

//默认文件最大大小
const size_t	CI_INIT_MAX_SIZE = 10485760;//10M
//默认路径
const string	CI_INIT_PATH     = "./";
//默认扩展名
const string	CI_INIT_EXTNAME  = ".log";
//默认模式
const log_mode_t	CI_INIT_MODE     = LM_LOOP_SIZE;
//默认文件时间格式
const string	CI_INIT_DATE_FORMAT = "%Y%m%d%H%M%S";
//默认换行符
#ifdef CC_PF_LINUX
	const string   CI_INIT_FEED = "\n";
#elif (defined CC_PF_MINGW32) || (defined CC_PF_WIN32) || (defined CC_PF_MFC)
	const string   CI_INIT_FEED = "\r\n";
#endif

typedef struct ClInfo
{
	string		m_name;         //日志名字
    log_mode_t  m_mode;         //日志模式
	ofstream*	m_output;       //日志输出流

	string m_prefix;       //文件前缀
	string m_postfix;      //文件后缀
	string m_extname;      //文件扩展名

	string m_feed;         //文件换行符
	string m_path;         //文件夹路径
	string m_curr_path;    //文件当前路径
	string m_date_format;  //文件时间格式

	time_t m_start;        //开始时间
	time_t m_stop;         //结束时间

	size_t m_size;         //日志大小
	size_t m_max_size;     //最大大小

    bool        m_is_work;      //是否工作
    bool        m_is_open;      //流是否打开
    bool        m_is_loop;      //是否循环

	ClInfo(const string& name,
		   const string& path = CI_INIT_PATH,
		   const string& feed = CI_INIT_FEED,
           const log_mode_t& mode = CI_INIT_MODE);

    ~ClInfo();

	const string& getCurrPath();

    bool startWork();

    inline void stopWork()
    {
        m_is_work = false;
        m_stop    = std::time(0);
    }

    inline bool isWork()
    {
        return m_is_work;
    }

    bool openOutput();

    /*
     * 检测是否需要重新打开文件，或者关闭文件打开标志
    */
    bool check();

	size_t write(const string& str);

	size_t write(const string& time_prompt, const string& str);

	//ClInfo& operator <<(const string& str);
}*ClInfoPtr;

//cpplogger namespace
CL_NAMESPACE_END

#endif // CLINFO_H
