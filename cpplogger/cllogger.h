/*
 * 日期：2015-8-7
 * 作者：blackcat
 * 描述：该文件是cpplogger的日志类文件
 *
*/

/*
 * 修订记录
*/

/*
 * 日期：2015-8-7
 * 修订内容：
*/
#ifndef CLLOGGER_H
#define CLLOGGER_H

#include <cltypedef.h>
#include <clinforegistry.h>
#include <ccseqqueue.h>

#if __cplusplus >= 201103L
#   include <sstream>
#endif

//cpplogger namespace
CL_NAMESPACE_BEGIN

/*
 * 日志消息
*/
struct ClMessage
{
	size_t   m_id;
	string m_msg;

    ClMessage()
        :m_id(0), m_msg()
    {}

	ClMessage(size_t id, string msg)
        :m_id(id), m_msg(msg)
    {}

    void reset(){m_id = 0;}
};

//日志write间隔
const int CL_INIT_WRITE_INTERVAL = 200;//ms
//日志check间隔
const int CL_INIT_CHECK_INTERVAL = 2000;//ms
//日志停止之前的睡眠时间
const int CL_INIT_STOP_TIMES     = 1000000;//1s
//日志停止之前继续执行的次数
const size_t  CL_INIT_STOP_COUNT	   = 5;

class ClLogger
{
public:
    ClLogger();

    //构造函数
    //日志队列的size
    //日志操作的等待时间
	explicit ClLogger(size_t queue_size, int queue_waits);

    //启动所有的日志
    bool start();

    //停止日志
    void stop();

    //启动日志
    //根据id或者name
	bool start(const size_t& id);
	bool start(const string& name);

	void stop(const size_t& id);
	void stop(const string& name);

    //注册日志类型信息
	size_t registerLogInfo(const ClInfoPtr& clinfo);

	size_t registerLogInfo(const string &name);

	size_t registerLogInfo(const string& name,
							  const string& prefix,
							  const string& postfix);

	size_t registerLogInfo(const string& name,
							  const string& path,
							  const string& feed,
                              const log_mode_t& mode);

	size_t registerLogInfo(const string& name,
							  const string& prefix,
							  const string& postfix,
							  const string& path,
							  const string& feed,
                              const log_mode_t& mode);

    //写入函数
    //C风格的格式化函数
	bool writef(const size_t& id, const char* format, ...);
	bool writef(const string& name, const char* format, ...);

    //c++模板实现的printf
    //writef_s("logname", "currdata :% ->time:%", data_want_write, time(0));
#if __cplusplus >= 201103L
    template <typename... Args>
	bool writef_s(const size_t& id, const char* format, Args ... args)
    {
        if(id == 0 || id > m_infos_reg.size()){
            return false;
        }

        if(!m_infos_reg[id]->isWork()){
            return false;
        }

        std::ostringstream out;

        cl_printf(out, format, args ...);

        ClMessage message(id, out.str());

        m_msg_queue.push(message);

        return true;
    }

    template <typename... Args>
	bool writef_s(const string& name, const char* format, Args ... args)
    {
		size_t id = m_infos_reg.getLogId(name);

        if(id == 0 || !m_infos_reg[id]->isWork()){
            return false;
        }

        std::ostringstream out;

        cl_printf(out, format, args ...);

        ClMessage message(id, out.str());

        m_msg_queue.push(message);

        return true;
    }
#endif

    //写入单个string
	bool write(const size_t& id, const string& msg);
	bool write(const string& name, const string& msg);

    //c++模板实现的多参数写入
    //mwrite(logid, "currdata :", data_want_write,  " ->time:", time(0)); //效果同上
#if __cplusplus >= 201103L
    template <typename... Args>
	bool mwrite(const size_t& id, Args ... args)
    {
        if(id == 0 || id > m_infos_reg.size()){
            return false;
        }

        if(!m_infos_reg[id]->isWork()){
            return false;
        }

        std::ostringstream out;

        cl_print(out, args ...);

        ClMessage message(id, out.str());

        m_msg_queue.push(message);

        return true;
    }

    template <typename... Args>
	bool mwrite(const string& name, Args ... args)
    {
		size_t id = m_infos_reg.getLogId(name);

        if(id == 0 || !m_infos_reg[id]->isWork()){
            return false;
        }

        std::ostringstream out;

        cl_print(out, args ...);

        ClMessage message(id, out.str());

        m_msg_queue.push(message);

        return true;
    }
#endif

public:
    //add more setXX

protected:
    //启动write以及check线程
    bool __start();

    //辅助函数
	bool __startThread(cc::thread_t& thread_t, cc::thread_func* pfunc, cc::thread_para_t para);

protected:
    //write线程
	static cc::thread_ret_t CC_THREAD s_write(cc::thread_para_t para);
    //check线程
	static cc::thread_ret_t CC_THREAD s_check(cc::thread_para_t para);

	cc::thread_t     m_write_thr;
	cc::thread_t     m_check_thr;

	int      m_write_interval;
	int      m_check_interval;
	size_t       m_stop_times;
	int      m_stop_count;

    bool    m_write_flag;
    bool    m_check_flag;

	cc::SeqQueue<ClMessage>   m_msg_queue;
    ClInfoRegistry          m_infos_reg;
};

//cpplogger namespace
CL_NAMESPACE_END

/*export cpplogger*/
typedef cl::ClLogger CppLogger;

#endif // CLLOGGER_H
