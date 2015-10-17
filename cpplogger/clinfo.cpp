#include "clinfo.h"
#include <ccpublic.h>
#include <ctime>

static bool s_global_isLoop(cl::log_mode_t mode)
{
    return mode < cl::LM_LOOP;
}

cl::ClInfo::ClInfo(const string &name, const string &path, const string &feed, const log_mode_t &mode)
    : m_name(name),
      m_mode(mode),
	  m_output(new ofstream),
      m_prefix(),
      m_postfix(),
      m_extname(CI_INIT_EXTNAME),
      m_feed(feed),
      m_path(path),
      m_curr_path(),
      m_date_format(CI_INIT_DATE_FORMAT),
      m_start(0),
      m_stop(0),
      m_size(0),
      m_max_size(CI_INIT_MAX_SIZE),
      m_is_work(false),
      m_is_open(false),
      m_is_loop(false)
{

}

cl::ClInfo::~ClInfo()
{
    if(m_is_open){
        m_output->close();
    }
    delete m_output;
}

const string &cl::ClInfo::getCurrPath()
{
    if(!m_is_open){
		m_curr_path = cc::formatPath(m_path);

        m_curr_path += m_name;
        if(!m_prefix.empty()){
            m_curr_path += '-';
        }
        m_curr_path += m_prefix;
        m_curr_path += '-';
		m_curr_path += cc::getLocalTimeStr(m_date_format);
        if(!m_postfix.empty()){
            m_curr_path += '-';
        }
        m_curr_path += m_postfix;
        m_curr_path += m_extname;
    }

    return m_curr_path;
}

bool cl::ClInfo::startWork()
{
    if(m_is_work){
        return true;
    }

    if(m_is_open && !m_is_work){
        return false;
    }

    m_is_work = true;
    m_is_open = openOutput();
    m_is_loop = s_global_isLoop(m_mode);
	m_start   = std::time(0);

    return m_is_work;
}

bool cl::ClInfo::openOutput()
{
    if(!m_is_work){
        return false;
    }

    if(m_is_open){
        return true;
    }

	const string& curr_path = this->getCurrPath();

	if(!cc::accessFile(m_path) && !cc::createDirectory(m_path)){
        return false;
    }

	m_output->open(curr_path.c_str(), std::ofstream::out | std::ofstream::ate);

    return m_output->is_open();
}

bool cl::ClInfo::check()
{
	static const time_t S_ONE_DAY  = 86400;

    switch(m_mode)
    {
        case LM_LOOP_SIZE:
        {
            if(m_size >= m_max_size){
                m_is_open = openOutput();
            }
        }break;
        case LM_LOOP_TIME:
        {
			time_t cur_time = std::time(0);

            if(cur_time - m_start >= S_ONE_DAY){
                m_is_open = openOutput();
            }
        }break;
        case LM_ONCE_SIZE:
        {
            if(m_size >= m_max_size)
                m_is_open = m_is_work = false;
        }break;
        case LM_ONCE_TIME:
        {
			time_t cur_time = std::time(0);

            if(cur_time - m_start >= S_ONE_DAY){
                m_is_open = m_is_work = false;
            }
        }break;
    default:
        break;
    }

    return m_is_open;
}

size_t cl::ClInfo::write(const string &str)
{
    *m_output << str << m_feed;

    m_output->flush();

	size_t write_size = str.size() + m_feed.size();

    m_size += write_size;

    return write_size;
}

size_t cl::ClInfo::write(const string &time_prompt, const string &str)
{
    *m_output << time_prompt << str << m_feed;

    m_output->flush();

	size_t write_size = time_prompt.size() + str.size() + m_feed.size();

    m_size += write_size;

    return write_size;
}


