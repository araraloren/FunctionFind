

#include <cllogger.h>
#include <stdarg.h>

#ifdef CC_PF_WIN32
#	include <process.h>
#endif


cl::ClLogger::ClLogger()
    : m_write_interval(CL_INIT_WRITE_INTERVAL),
      m_check_interval(CL_INIT_CHECK_INTERVAL),
      m_stop_times(CL_INIT_STOP_TIMES),
      m_stop_count(CL_INIT_STOP_COUNT),
      m_write_flag(false),
      m_check_flag(false),
      m_msg_queue()
{
    m_msg_queue.init();
}

cl::ClLogger::ClLogger(size_t queue_size, int queue_waits)
    : m_write_interval(CL_INIT_WRITE_INTERVAL),
      m_check_interval(CL_INIT_CHECK_INTERVAL),
      m_stop_times(CL_INIT_STOP_TIMES),
      m_stop_count(CL_INIT_STOP_COUNT),
      m_write_flag(false),
      m_check_flag(false),
      m_msg_queue(queue_size, queue_waits)
{
    m_msg_queue.init();
}

bool
cl::ClLogger::start()
{
    bool ret = false;

    if (__start()) {
        for(std::vector<ClInfoPtr>::iterator it = m_infos_reg.begin();
                it != m_infos_reg.end();it ++){
            cl::ClInfoPtr& cip = *it;

            ret = ret && cip->startWork();
        }
    }

    return ret;
}

void
cl::ClLogger::stop()
{
	cc::microSleep(m_stop_times);
    m_write_flag = m_check_flag = false;
#if __cplusplus >= 201103L
    m_write_thr.detach();
    m_check_thr.join();
#else
#   if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32)
        pthread_detach(m_write_thr);
        pthread_join(m_check_thr, 0);
#	elif (defined CC_PF_WIN32)
	static const int S_WAIT_FOR = 5000;

	if (WAIT_TIMEOUT == WaitForSingleObject(m_check_thr, S_WAIT_FOR)){
		TerminateThread(m_check_thr, 1);
	}
#   endif
#endif
}

bool
cl::ClLogger::start(const size_t &id)
{
    if(id == 0 || id > m_infos_reg.size()){
        return false;
    }

    if(__start()){
        return m_infos_reg[id]->startWork();
    }else{
        return false;
    }
}

bool
cl::ClLogger::start(const string &name)
{
    ClInfoPtr curr_info = m_infos_reg[name];

    if(curr_info == nullptr){
        return false;
    }

    if(__start()){
        return curr_info->startWork();
    }else{
        return false;
    }
}

void
cl::ClLogger::stop(const size_t &id)
{
    if(id == 0 || id > m_infos_reg.size()){
        return;
    }

    if(m_write_flag && m_check_flag){
        m_infos_reg[id]->stopWork();
    }
}

void
cl::ClLogger::stop(const string &name)
{
    ClInfoPtr curr_info = m_infos_reg[name];

    if(curr_info == nullptr){
        return;
    }

    if(m_write_flag && m_check_flag){
        curr_info->stopWork();
    }
}

size_t
cl::ClLogger::registerLogInfo(const cl::ClInfoPtr &clinfo)
{
    return m_infos_reg.registerInfo(clinfo);
}

size_t
cl::ClLogger::registerLogInfo(const string &name)
{
    ClInfoPtr clinfo = new ClInfo(name);

    return m_infos_reg.registerInfo(clinfo);
}

size_t
cl::ClLogger::registerLogInfo(const string &name, const string &prefix, const string &postfix)
{
    ClInfoPtr clinfo = new ClInfo(name);

    clinfo->m_prefix = prefix;
    clinfo->m_postfix = postfix;

    return m_infos_reg.registerInfo(clinfo);
}

size_t
cl::ClLogger::registerLogInfo(const string &name, const string &path, const string &feed, const cl::log_mode_t &mode)
{
    ClInfoPtr clinfo = new ClInfo(name, path, feed, mode);

    return m_infos_reg.registerInfo(clinfo);
}

size_t
cl::ClLogger::registerLogInfo(const string &name, const string &prefix, const string &postfix, const string &path, const string &feed, const cl::log_mode_t &mode)
{
    ClInfoPtr clinfo = new ClInfo(name, path, feed, mode);

    clinfo->m_prefix = prefix;
    clinfo->m_postfix = postfix;

    return m_infos_reg.registerInfo(clinfo);
}

bool
cl::ClLogger::writef(const size_t &id, const char *format, ...)
{
	static const size_t S_BUFF_SIZE = 4096;

    if(id == 0 || id > m_infos_reg.size()){
        return false;
    }

    if(!m_infos_reg[id]->isWork()){
        return false;
    }

    va_list argslist;
    char    buff[S_BUFF_SIZE];

	int ret = 0;

    va_start(argslist, format);
#if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32)
	ret = std::vsnprintf(buff, S_BUFF_SIZE, format, argslist);
#elif (defined CC_PF_WIN32)
	ret = _vsnprintf_s(buff, S_BUFF_SIZE, format, argslist);
#endif
    va_end(argslist);

    if(ret <= 0){
        return false;
    }

	ClMessage message(id, string(buff, ret));

    m_msg_queue.push(message);

    return true;
}

bool
cl::ClLogger::writef(const string &name, const char *format, ...)
{
	static const size_t S_BUFF_SIZE = 4096;

	size_t id = m_infos_reg.getLogId(name);

    if(id == 0 || !m_infos_reg[id]->isWork()){
        return false;
    }

    va_list argslist;
    char    buff[S_BUFF_SIZE];

	int ret = 0;

    va_start(argslist, format);
#if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32)
    ret = std::vsnprintf(buff, S_BUFF_SIZE, format, argslist);
#elif (defined CC_PF_WIN32)
	ret = _vsnprintf_s(buff, S_BUFF_SIZE, format, argslist);
#endif
    va_end(argslist);

    if(ret <= 0){
        return false;
    }

	ClMessage message(id, string(buff, ret));

    m_msg_queue.push(message);

    return true;
}

bool
cl::ClLogger::write(const size_t &id, const string &msg)
{
    if(id == 0 || id > m_infos_reg.size()){
        return false;
    }

    if(!m_infos_reg[id]->isWork()){
        return false;
    }

    ClMessage message(id, msg);

    m_msg_queue.push(message);

    return true;
}

bool
cl::ClLogger::write(const string &name, const string &msg)
{
	size_t id = m_infos_reg.getLogId(name);

    if(id == 0 || !m_infos_reg[id]->isWork()){
        return false;
    }

    ClMessage message(id, msg);

    m_msg_queue.push(message);

    return true;
}

bool
cl::ClLogger::__start()
{
    if(m_write_flag && m_check_flag){
        return true;
    }

    if(!m_write_flag){
        m_write_flag = true;
		if(!__startThread(m_write_thr, cl::ClLogger::s_write, static_cast<cc::thread_para_t>(this))){
            m_write_flag = false;
            return false;
        }
    }

    if(!m_check_flag){
        m_check_flag = true;
		if(!__startThread(m_check_thr, cl::ClLogger::s_check, static_cast<cc::thread_para_t>(this))){
            m_check_flag = false;
            return false;
        }
    }

    return true;
}

bool
cl::ClLogger::__startThread(cc::thread_t &thread_t, cc::thread_func *pfunc, cc::thread_para_t para)
{
#if __cplusplus >= 201103L
	cc::thread_t thr(pfunc, para);

    thread_t.swap(thr);
#else
#   if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32)
		int ret = pthread_create(&thread_t, 0, pfunc, para);

        if(ret != 0){
            return false;
        }
#   elif (defined CC_PF_WIN32)
	thread_t = (cc::thread_t)_beginthreadex(0, 0, pfunc, para, 0, 0);
#   endif
#endif

    return true;
}

cc::thread_ret_t CC_THREAD cl::ClLogger::s_write(cc::thread_para_t para)
{
    cl::ClLogger* logger = static_cast<cl::ClLogger*>(para);

	cc::SeqQueue<cl::ClMessage>& msgqueue = logger->m_msg_queue;
    cl::ClInfoRegistry& registry = logger->m_infos_reg;

    cl::ClMessage message;

    bool&        is_write = logger->m_write_flag;

	int&  sleep_tm = logger->m_write_interval;

	int  quit_wait = logger->m_stop_count;

    for(;is_write || quit_wait;is_write ? quit_wait : quit_wait --){
        msgqueue.pop(message);

        ClInfoPtr curr_info = registry[message.m_id];

        curr_info->write(message.m_msg);

        message.reset();

		cc::microSleep(sleep_tm);
    }

	return static_cast<cc::thread_ret_t>(0);
}

cc::thread_ret_t CC_THREAD cl::ClLogger::s_check(cc::thread_para_t para)
{
    cl::ClLogger* logger = static_cast<cl::ClLogger*>(para);

    cl::ClInfoRegistry& registry = logger->m_infos_reg;

    bool&        is_check = logger->m_check_flag;

	int&  sleep_tm = logger->m_check_interval;

	int  quit_wait = logger->m_stop_count;

    for(;is_check || quit_wait;is_check ? quit_wait : quit_wait --){

        for(std::vector<ClInfoPtr>::iterator it = registry.begin();
                it != registry.end();it ++){
            cl::ClInfoPtr& cip = *it;

            if(cip && cip->isWork()){
                cip->check();
            }
        }

		cc::microSleep(sleep_tm);
    }

	return static_cast<cc::thread_ret_t>(0);
}
