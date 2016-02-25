#ifndef FFOPTION_H
#define FFOPTION_H

#include <string>
#include <ctime>
#include <cstring>
#include <ffconfig.h>
#include <fffunction.h>
#include <ccpublic.h>

NAMESPACE_FF_BEGIN

const size_t FF_ARGV_MAX = 128;

class FFOption
{
public:
    static FFOption*
    getInstance()
    {
        //in c++11 it's threadsafe
        static FFOption _instance;

        return &_instance;
    }

    bool
    parseCommandArgs(int argc, char** argv);

    void
    printOptionHelp() const;

private:
    FFOption();
    FFOption(const FFOption& );
    FFOption operator = (const FFOption&);
    ~FFOption()
    {
        for (size_t i = 0;i < m_args_c;i ++) {
            delete m_args[i];
        }
    }

    /*setOptions*/
public:

    inline void
    setIgnoreName(bool is_ignore_name)
    {
        this->m_ignore_name = is_ignore_name;
    }

    inline void
    setIgnoreRetType(bool is_ignore_ret_type)
    {
        this->m_ignore_ret = is_ignore_ret_type;
    }

    inline void
    setUseRegex(bool is_use_regex)
    {
        this->m_regex = is_use_regex;
    }

    inline void
    setRecursion(bool is_recursion_)
    {
        this->m_recursion = is_recursion_;
    }

    inline void
    setMatchDeclare(bool is_match_declare)
    {
        this->m_declare = is_match_declare;
    }

    inline void
    setPrintFilename(bool is_print_filename)
    {
        this->m_print_fn = is_print_filename;
    }

    inline void
    setPrintLineNumber(bool is_print_line_number)
    {
        this->m_print_ln = is_print_line_number;
    }

    inline void
    setPrintMatchCount(bool is_print_match_count)
    {
        this->m_print_mc = is_print_match_count;
    }

    inline void
    setStartTime(std::time_t start_time)
    {
        this->m_start = start_time;
    }

    inline void
    setEndTime(std::time_t end_time)
    {
        this->m_end = end_time;
    }

    inline void
    setThreadCount(std::size_t thread_count)
    {
        this->m_jobs_t = thread_count;
    }

    inline void
    setFunctionSignature(const FunctionSignature& fs)
    {
        this->m_signature = fs;
    }

    inline void
    setClangArgs(const std::string& args)
    {
        this->checkClangOptions(args);
    }

    inline void
    setFiles(const std::vector<std::string>& files)
    {
        this->m_files = files;
    }

    inline void
    pushFiles(const std::string& file)
    {
        this->m_files.push_back(file);
    }

    /*getOptions*/
public:

    inline bool
    isIgnoreName() const
    {
        return this->m_ignore_name;
    }

    inline bool
    isIgnoreRetType() const
    {
        return this->m_ignore_ret;
    }

    inline bool
    isUseRegex() const
    {
        return this->m_regex;
    }

    inline bool
    isRecursion() const
    {
        return this->m_recursion;
    }

    inline bool
    isMatchDeclare() const
    {
        return this->m_declare;
    }

    inline bool
    isPrintFilename() const
    {
        return this->m_print_fn;
    }

    inline bool
    isPrintLineNumber() const
    {
        return this->m_print_ln;
    }

    inline bool
    isPrintMatchCount() const
    {
        return this->m_print_mc;
    }

    inline std::time_t
    startTime() const
    {
        return this->m_start;
    }

    inline std::time_t
    endTime() const
    {
        return this->m_end;
    }

    inline std::size_t
    threadCount() const
    {
        return this->m_jobs_t;
    }

    inline const FunctionSignature&
    functionSignature() const
    {
        return this->m_signature;
    }

    inline char* const *
    clangArgs() const
    {
        return this->m_args;
    }

    inline const std::vector<std::string>
    files() const
    {
        return this->m_files;
    }

public:
//#ifdef FF_DEBUG
    void
    debugPrintStatus() const;
//#endif
public:
    void
    checkClangOptions(const std::string& str)
    {
        if (m_args_c > 0) {
            return ;
        }
        std::vector<std::string> arglist = cc::spiltString(str, ',', cc::trim);

        m_args_c = arglist.size();
        for (size_t i = 0;i < arglist.size();i ++) {
            std::string &arg = arglist[i];

            m_args[i] = new char[arg.size() + 1];
            std::memcpy(m_args[i], arg.c_str(), arg.size());
            *m_args[arg.size()] = '\0';
        }
    }

    /*switch*/
private:
    /**
     * @brief m_ignore_name
     * ignore func name
     */
    bool    m_ignore_name;

    bool    m_ignore_ret;

    bool    m_regex;

    bool    m_recursion;

    bool    m_declare;

    bool    m_print_fn;

    bool    m_print_ln;

    bool    m_print_mc;

    /*options*/
private:
    std::time_t   m_start;

    std::time_t   m_end;

    int    m_jobs_t;

    FunctionSignature   m_signature;

    size_t m_args_c;

    char* m_args[FF_ARGV_MAX];

    std::vector<std::string> m_files;
};

NAMESPACE_FF_END

#endif // FFOPTION_H
