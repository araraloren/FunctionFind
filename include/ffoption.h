#ifndef FFOPTION_H
#define FFOPTION_H

#include <string>
#include <ctime>
#include <ffconfig.h>
#include <fffunction.h>

NAMESPACE_FF_BEGIN

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
    ~FFOption(){}

    /*setOptions*/
public:

    inline void
    setIgnoreName(bool is_ignore_name)
    {
        this->ignore_name_ = is_ignore_name;
    }

    inline void
    setIgnoreRetType(bool is_ignore_ret_type)
    {
        this->ignore_ret_type_ = is_ignore_ret_type;
    }

    inline void
    setUseRegex(bool is_use_regex)
    {
        this->regex_ = is_use_regex;
    }

    inline void
    setRecursion(bool is_recursion_)
    {
        this->recursion_ = is_recursion_;
    }

    inline void
    setMatchDeclare(bool is_match_declare)
    {
        this->find_declare_ = is_match_declare;
    }

    inline void
    setPrintFilename(bool is_print_filename)
    {
        this->print_filename_ = is_print_filename;
    }

    inline void
    setPrintLineNumber(bool is_print_line_number)
    {
        this->print_line_num_ = is_print_line_number;
    }

    inline void
    setPrintMatchCount(bool is_print_match_count)
    {
        this->print_match_count_ = is_print_match_count;
    }

    inline void
    setStartTime(std::time_t start_time)
    {
        this->start_time_ = start_time;
    }

    inline void
    setEndTime(std::time_t end_time)
    {
        this->end_time_ = end_time;
    }

    inline void
    setThreadCount(std::size_t thread_count)
    {
        this->threads_ = thread_count;
    }

    inline void
    functionSignature(const FunctionSignature& fs)
    {
        this->function_signature_ = fs;
    }

    /*getOptions*/
public:

    inline bool
    isIgnoreName() const
    {
        return this->ignore_name_;
    }

    inline bool
    isIgnoreRetType() const
    {
        return this->ignore_ret_type_;
    }

    inline bool
    isUseRegex() const
    {
        return this->regex_;
    }

    inline bool
    isRecursion() const
    {
        return this->recursion_;
    }

    inline bool
    isMatchDeclare() const
    {
        return this->find_declare_;
    }

    inline bool
    isPrintFilename() const
    {
        return this->print_filename_;
    }

    inline bool
    isPrintLineNumber() const
    {
        return this->print_line_num_;
    }

    inline bool
    isPrintMatchCount() const
    {
        return this->print_match_count_;
    }

    inline std::time_t
    startTime() const
    {
        return this->start_time_;
    }

    inline std::time_t
    endTime() const
    {
        return this->end_time_;
    }

    inline std::size_t
    threadCount() const
    {
        return this->threads_;
    }

    inline const FunctionSignature&
    functionSignature() const
    {
        return this->function_signature_;
    }

public:
//#ifdef FF_DEBUG
    void
    debugPrintStatus() const;
//#endif

    /*switch*/
private:
    bool    ignore_name_;

    bool    ignore_ret_type_;

    bool    regex_;

    bool    recursion_;

    bool    find_declare_;

    bool    print_filename_;

    bool    print_line_num_;

    bool    print_match_count_;

    /*options*/
private:
    std::time_t   start_time_;

    std::time_t   end_time_;

    int    threads_;

    FunctionSignature   function_signature_;
};

NAMESPACE_FF_END

#endif // FFOPTION_H