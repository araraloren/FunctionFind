#ifndef GETOPTION_H
#define GETOPTION_H

#include <ctime>
#include <vector>
#include <string>
#include <cstring>
#include <ffconfig.h>
#include <ccpublic.h>
#include <fffunction.h>
#include <ffparser.h>

using std::vector;
using std::string;
using std::time_t;

NAMESPACE_FF_BEGIN

const size_t FF_ARGS_MAX = 128;

/*
 * CFILE
*/
struct CFile {
    std::string path;
    std::int32_t eflag;
    CFile() :path(), eflag(0) {}
    CFile(const char* path, std::int32_t eflag): path(path), eflag(eflag) {}
    CFile(const std::string path, std::int32_t eflag): path(path), eflag(eflag) {}
};

struct CDir : public CFile {
    std::vector<std::string> exts;
};

class CommandOption
{
public:
    static CommandOption* getInstance()
    {
        static CommandOption s_instance;

        return &s_instance;
    }

    bool
    parseArgv(int argc, char ** argv);

    void
    help(const char* name, int err_msg_code) const;

    void
    debugOption() const;

public:
    const vector<CFile>&
    getFiles() const { return m_files; }

    const vector<Signature>&
    getSignatures() const { return m_signatures; }

public:
    bool is_ignore_case() const { return m_ignore_case; }

    bool is_recursive() const { return m_recursive; }

    bool is_print_filename() const { return m_print_fn; }

    bool is_print_linenumber() const { return m_print_ln; }

    bool is_print_offset() const { return m_print_off; }

    bool is_print_columnnumber() const { return m_print_col; }

    bool is_print_onlycount() const { return m_print_oc; }

public:
    time_t getStartTime() const { return m_start_tm; }

    time_t getEndTime() const { return m_end_tm; }

    int getJobThread() const { return m_job_t; }

    int getArgsCount() const { return m_args_cnt; }

    const char* getArgs(int index) const { return m_args[index]; }

    const string& getClassName() const { return m_class_name; }

    const string& getStructName() const { return m_struct_name; }

private:
    /* constructor */
    CommandOption();

    /* copy constructor */
    CommandOption(const CommandOption& );

    /* assignment operator */
    CommandOption operator = (const CommandOption&);

    /* destructor */
    ~CommandOption();

    /* main switch && data */
private:
    enum TypeSwitch {
        TSFUNC = 0x0001,
        TSDECL = 0x0002,
    };

    /**
     * @brief m_ts
     */
    TypeSwitch m_ts;

    /**
     * @brief m_files
     * - files will be parse
     */
    vector<CFile> m_files;

    /**
     * @brief m_signatures
    */
    vector<Signature> m_signatures;

    /* switch */
private:
    /**
     * @brief m_ignore_case
     * - ignore case of function name
     * - ignore case of arg name
     * - default is false
     */
    bool m_ignore_case;

    /**
     * @brief m_recursion
     * - recursion search file in directory
     * - default is false
     */
    bool m_recursive;

    /**
     * @brief m_print_line
     * - print match line
     * - default is true
     * - will disable by --only-count
     */
    //bool m_print_line;

    /**
     * @brief m_print_ln
     * - print file name
     * - default is true
     * - will disable by --only-count, but you can specify it as true
     */
    bool m_print_fn;

    /**
     * @brief m_print_ln
     * - print line number
     * - default is false
     * - will forbidden by --only-count
     */
    bool m_print_ln;

    /**
     * @brief m_print_off
     * - print offset of whole file
     * - default is false
     * - will forbidden by --only-count
     */
    bool m_print_off;

    /**
     * @brief m_print_col
     * - print column number
     * - default is false
     * - will forbidden by --only-count
     */
    bool m_print_col;

    /**
     * @brief m_print_oc
     * - print only count
     * - default is false
     */
    bool m_print_oc;

    /* option */
private:
    /**
     * @brief m_start_tm
     */
    time_t m_start_tm;

    /**
     * @brief m_end_tm
     */
    time_t m_end_tm;

    /**
     * @brief m_job_t
     */
    int m_job_t;

    /**
     * @brief m_args_cnt
     */
    int m_args_cnt;

    /**
     * @brief m_args
     */
    char* m_args[FF_ARGS_MAX];

    /**
     * @brief m_class_name
     * - specified struct or class name
     */
    string m_class_name;

    /**
     * @brief m_struct_name
     * - specified struct or class name
     */
    string m_struct_name;

    /* option helper */
private:
    enum FileType {
        FT_OTHER=-1,
        FT_C    = 0x00000001,
        FT_CPP  = 0x00000002,
        FT_C__  = 0x00000004, //c++
        FT_CXX  = 0x00000008,
        FT_H    = 0x00000016,
        FT_HPP  = 0x00000032,
        FT_FILE = 0x00000064,
        FT_DIR  = 0x00000128,
        FT_NOEXT= 0x00000256,
    };

    FileType
    getFileType(const char* path);

    FileType
    getExtType(const char* pext);

    bool
    isBeginWithMinus(const char* str);

    bool
    isExtFlag(const char* str);

    bool
    genTypeSwitch(const char* str);

    bool
    genFiles(const char* str);

    time_t
    toTimet(const char* str);
};

NAMESPACE_FF_END

#endif // GETOPTION_H
