#include "ffoption.h"

#include <getopt.h>

NAMESPACE_FF_BEGIN

CommandOption::CommandOption()
    :m_ts(TSFUNC)
    ,m_files()
    ,m_ignore_case(false)
    ,m_recursive(false)
    ,m_print_fn(false)
    ,m_print_ln(false)
    ,m_print_off(false)
    ,m_print_col(false)
    ,m_print_oc(false)
    ,m_start_tm(0)
    ,m_end_tm(0)
    ,m_job_t(1)
    ,m_args_cnt(0)
    ,m_args()
    ,m_class_name()
    ,m_struct_name()
{

}

CommandOption::~CommandOption()
{
    for (int i = 0;i < m_args_cnt;i ++) {
        delete m_args[i];
    }
}

CommandOption::FileType
CommandOption::getFileType(const char *path)
{
    if (!cc::accessFile(path)) { return FT_OTHER; }

    if (cc::isDirectory(path)) { return FT_DIR; }

    const char* pext = cc::getExtname(path);

    if (nullptr == pext) {
        return FT_NOEXT;
    }

    return getExtType(pext);
}

CommandOption::FileType
CommandOption::getExtType(const char *pext)
{
    static const char* s_default_ext[] = {
        "c",    "cpp",  "cxx",  "c++",
        "h",    "hpp",
    };

    static size_t s_len = sizeof(s_default_ext) / sizeof(const char*);

    size_t i = 0;
    for (;i < s_len;i ++) {
        if (!std::strcmp(*(s_default_ext + i), pext)) {
            break;
        }
    }

    if (i < s_len) {
        return static_cast<FileType>(0x00000001 << i);
    }

    return FT_FILE;
}

bool CommandOption::isBeginWithMinus(const char *str)
{
    return (str && *str == '-');
}

bool CommandOption::isExtFlag(const char *str)
{
    return isBeginWithMinus(str) && (std::strcmp(str, "-e") == 0);
}

bool CommandOption::genTypeSwitch(const char *str)
{
    if (!str) return false;

    for(;*str != '\0';str ++) {
        switch (*str) {
            case 'f': m_ts = (TypeSwitch)(m_ts | TSFUNC);
            case 'b': m_ts = (TypeSwitch)(m_ts | TSDECL);
        case '-':break;
        default:
            return false;
        }
    }

    return true;
}

bool CommandOption::genFiles(const char *str)
{

    size_t len = std::strlen(str);

    if (*(str + len - 1) == '/') {
        //m_files += cc::searchDirectory(str, recursive);
    }

    const char* pext = cc::getExtname(str);

    if (pext) {
        //for (size_t i = 0;i < sizeof(s_ext_table) / sizeof(const char*);
        //        i ++) {
        //    if (std::strcmp(pext, *(s_ext_table + i)) == 0) {
                // push back to files
        //    }
        //}
    }

    return true;
}

bool CommandOption::parseArgv(int argc, char **argv)
{
    enum eIndex {
        EI_SEARCH_TYPE = 0,
        EI_EXTENSION,
    };

    // argv[0] for help
    int cur = 1;

    // generate type switch
    if (argc <= cur || !isBeginWithMinus(*(argv + cur)) || !genTypeSwitch(*(argv + cur))) {
        help(argv[0], EI_SEARCH_TYPE); //error
        return false;
    }

    std::vector<CDIR> dofs;

    //generate files
    for (FileType ft = FT_OTHER;argc <= cur;cur ++) {
        if (isBeginWithMinus(*(argv + cur))) {
            break;
        }
        if ((ft = getFileType(*(argv + cur))) != FT_OTHER) {
            CDIR dof;

            dof.path    = *(argv + cur);
            dof.eflag   |= ft;

            if (ft == FT_DIR) {
                cur ++; // move to extension name args -e
                if (isExtFlag(*(argv + cur))) {
                    cur ++; // process extension name
                    dof.exts = cc::splitString(*(argv + cur), ',', cc::trim);
                } else {
                    cur ++;
                    if (FT_OTHER != getFileType(*(argv + cur))) {
                        help(argv[0], EI_EXTENSION); // error
                        return false;
                    }
                }
            }

            dofs.push_back(dof);
        }
    }

    //
    static struct option long_options[] = {
        {"ignore-case",     no_argument,        0, 'i'},
        {"recursive",       no_argument,        0, 'r'},
        {"print-filename",  no_argument,        0, 'f'},
        {"print-linenum",   no_argument,        0, 'n'},
        {"print-offset",    no_argument,        0, 'o'},
        {"print-column",    no_argument,        0, 'c'},
        {"help",            no_argument,        0, 'h'},
        {"version",         no_argument,        0, 'v'},
        {"only-count",      no_argument,        0, 't'},

        {"start-time",      required_argument,  0, 'S'},
        {"end-time",        required_argument,  0, 'E'},
        {"thread",          required_argument,  0, 'J'},
        {"clang-args",      required_argument,  0, 'A'},
        {"class",           required_argument,  0, 'C'},
        {"struct",          required_argument,  0, 'T'},
        {0, 0, 0, 0}
    };

    int opt = 0, opt_index = 0, argcount = argc - cur;

    const char* clang_args = nullptr;

    for (;;) {
        opt = getopt_long(argcount, &argv[cur], "irfnochvtS:E:J:A:C:T:", long_options, &opt_index);

        if (opt == -1) {
            break;
        }

        switch(opt) {
        case 'i': m_ignore_case = true; break;
        case 'r': m_recursive   = true; break;
        case 'f': m_print_fn    = true; break;
        case 'n': m_print_ln    = true; break;
        case 'o': m_print_off   = true; break;
        case 'c': m_print_col   = true; break;
        case 't': m_print_oc    = true; break;

        case 'S': m_start_tm    = std::atol(optarg); break;
        case 'E': m_end_tm      = std::atol(optarg); break;
        case 'J': m_job_t       = std::atoi(optarg); break;
        case 'C': m_class_name  = std::string(optarg); break;
        case 'T': m_struct_name = std::string(optarg); break;
        case 'A': clang_args    = optarg; break;

        case 'v': /* show version */ break;
        case 'h':
        default:
            /* call help and exit */
            break;
        }
    }

    // if dotfs is empty read file from stdin
    if (dofs.empty()) {
        char stdinfile[2048];
        FileType ft;

        while (std::fgets(stdinfile, 2048, stdin)) {
            stdinfile[std::strlen(stdinfile) - 1] = '\0';
            if ((ft = getFileType(stdinfile)) != FT_OTHER) {
                CDIR dof;

                dof.path    = stdinfile;
                dof.eflag   |= ft;

                dofs.push_back(dof);
            }
        }
    }

    //process dofs when we know -r
    std::vector<string> res;
    for (std::vector<CDIR>::iterator it = dofs.begin(); \
         it != dofs.end();it ++) {
        // search file
        if (it->eflag & FT_DIR) {
            if (it->exts.empty()) {
                res = cc::searchDirectory(it->path, m_recursive);
            } else {
                //search file by extension
                res = cc::searchDirectory(it->path, m_recursive, it->exts);
            }

            for (std::vector<string>::iterator jt = res.begin();
                 jt != res.end(); jt ++) {
                m_files.push_back(CFILE(*jt, getFileType(jt->c_str())));
            }
            // continue to CDIR
            continue;
        }
        // file
        m_files.push_back(CFILE(it->path, it->eflag));
    }

    if (clang_args) {
        // process clang args
    }

    //generate function signature
    if (optind >= argcount) {
        help(argv[0], 2);
        return false;
    } else {
        while (optind < argcount) {
            // read from optional

        }
    }

    return true;
}

void CommandOption::debugOption()
{
    std::printf("debug optional ---> \n");

    std::printf("type switch -> ");
    switch (m_ts) {
    case TSFUNC: std::printf("function");
    case TSDECL: std::printf("declare");
    }


}

void CommandOption::help(const char *name, int err_msg_code)
{
    const char* s_msg[] = {
        "Missing search type.",
        "-e Missing extension name.",
        "Missing function signature."
    };
    std::fprintf(stderr, "%s\n", s_msg[err_msg_code]);
    std::printf("cur name -> %s\n", name);
}

NAMESPACE_FF_END
