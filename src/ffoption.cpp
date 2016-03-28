#include "ffoption.h"

#include <getopt.h>

NAMESPACE_FF_BEGIN

CommandOption::CommandOption()
    :m_ts(TSFUNC)
    ,m_files()
    ,m_signatures()
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
        case 'f': m_ts = (TypeSwitch)(m_ts | TSFUNC); break;
        case 'd': m_ts = (TypeSwitch)(m_ts | TSDECL); break;
        case '-': break;
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
        EI_HELP = -1,
        EI_SEARCH_TYPE = 0,
        EI_EXTENSION = 1,
        EI_OPTIONAL = 2,
        EI_SIGNATURE = 3,
    };

    // argv[0] for help
    int cur = 1;

    // generate type switch
    if (cur >= argc || !isBeginWithMinus(*(argv + cur)) || !genTypeSwitch(*(argv + cur))) {
        if (cur >= argc || !std::strcmp(*(argv + cur), "-h") || !std::strcmp(*(argv + cur), "--help")) {
            help(argv[0], EI_HELP); //help exit
        }
        help(argv[0], EI_SEARCH_TYPE); //error exit
    }
    cur ++; // next is file

    vector<CDir> dofs;

    //generate files
    for (FileType ft = FT_OTHER;cur < argc;cur ++) {
        if (isBeginWithMinus(*(argv + cur))) {
            break;
        }
        if ((ft = getFileType(*(argv + cur))) != FT_OTHER) {
            CDir dof;

            dof.path    = *(argv + cur);
            dof.eflag   |= ft;

            if (ft == FT_DIR && cur < argc - 1) {
                cur ++; // move to extension name args -e
                if (isExtFlag(*(argv + cur)) && cur < argc - 1) {
                    cur ++; // process extension name
                    dof.exts = cc::splitString(*(argv + cur), ',', cc::trim);
                } else {
                    cur --;
                }
                /* else {
                    // if next is
                    if (FT_OTHER != getFileType(*(argv + cur))) {
                        help(argv[0], EI_EXTENSION); // error exit
                    }
                }*/
            }

            dofs.push_back(dof);
        } else {
            cur --; break;
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

        case 'S': m_start_tm    = toTimet(optarg); break;
        case 'E': m_end_tm      = toTimet(optarg); break;
        case 'J': m_job_t       = std::atoi(optarg); break;
        case 'C': m_class_name  = string(optarg); break;
        case 'T': m_struct_name = string(optarg); break;
        case 'A': clang_args    = optarg; break;

        case 'v': /* show version */ break;
        case 'h': help(argv[0], EI_HELP);
        default:
            /* call help and exit */
            help(argv[0], EI_OPTIONAL);
        }
    }

    // if dotfs is empty read file from stdin
    if (dofs.empty()) {
        char stdinfile[2048];
        FileType ft;

        while (std::fgets(stdinfile, 2048, stdin)) {
            stdinfile[std::strlen(stdinfile) - 1] = '\0';
            if ((ft = getFileType(stdinfile)) != FT_OTHER) {
                CDir dof;

                dof.path    = stdinfile;
                dof.eflag   |= ft;

                dofs.push_back(dof);
            }
        }
    }

    //process dofs
    vector<string> res;
    for (vector<CDir>::iterator it = dofs.begin(); \
         it != dofs.end();it ++) {
        // search file
        if (it->eflag & FT_DIR) {
            if (it->exts.empty()) {
                res = cc::searchDirectory(it->path, m_recursive);
            } else {
                //search file by extension
                res = cc::searchDirectory(it->path, m_recursive, it->exts);
            }

            for (vector<string>::iterator jt = res.begin();
                 jt != res.end(); jt ++) {
                m_files.push_back(CFile(*jt, getFileType(jt->c_str())));
            }
            // continue to CDIR
            continue;
        }
        // file
        m_files.push_back(CFile(it->path, it->eflag));
    }

    if (clang_args) {
        // process clang args
        vector<string> clangargs = cc::splitString(clang_args);

        m_args_cnt = static_cast<int>(clangargs.size());
        for (size_t i = 0;i < clangargs.size();i ++) {
            const string& args = clangargs[i];

            m_args[i] = new char[args.size() + 1];
            std::strncpy(m_args[i], args.c_str(), args.size() + 1);
        }
    }

    //generate function signature
    if (optind >= argcount) {
        help(argv[0], EI_SIGNATURE); // signature exit
    } else {
        Signature sign;

        for (;optind < argcount; optind ++) {
            // read from user optional
            sign = ff::SignatureParser::parse(string(argv[optind]));
            if (!sign.isValid()) {
                std::fprintf(stderr, "WARNING! %s is not recognize signature format\n", argv[optind]);
            } else {
                m_signatures.push_back(sign);
            }
        }
    }

    return true;
}

void CommandOption::debugOption() const
{
    std::fprintf(stderr, "debug optional ---> \n");

    std::fprintf(stderr, "%s\n", cc::repeat('*', 50).c_str());

    std::fprintf(stderr, "type switch -> ");
    switch (m_ts) {
    case TSFUNC: std::fprintf(stderr, "function");
    case TSDECL: std::fprintf(stderr, "declare");
    }
    std::fprintf(stderr, "\n");

    std::fprintf(stderr, "CFILE -> [");
    for(vector<CFile>::const_iterator it = m_files.begin();   \
        it != m_files.end();it ++) {
        std::fprintf(stderr, " %s -flag: %d ==", it->path.c_str(), it->eflag);
    }
    std::fprintf(stderr, " ]\n");

    std::fprintf(stderr, "ignore case -> %s\n", cc::toString(m_ignore_case).c_str());

    std::fprintf(stderr, "recursive -> %s\n", cc::toString(m_recursive).c_str());

    std::fprintf(stderr, "print funcname -> %s\n", cc::toString(m_print_fn).c_str());

    std::fprintf(stderr, "print line -> %s\n", cc::toString(m_print_ln).c_str());

    std::fprintf(stderr, "print offset -> %s\n", cc::toString(m_print_off).c_str());

    std::fprintf(stderr, "print column -> %s\n", cc::toString(m_print_col).c_str());

    std::fprintf(stderr, "print only count -> %s\n", cc::toString(m_print_oc).c_str());

    std::fprintf(stderr, "start time -> %ld\n", m_start_tm);

    std::fprintf(stderr, "end time -> %ld\n", m_end_tm);

    std::fprintf(stderr, "job thread -> %d\n", m_job_t);

    std::fprintf(stderr, "clang args -> [");
    for (int i = 0;i < m_args_cnt;i ++) {
        std::fprintf(stderr, " %s", m_args[i]);
    }
    std::fprintf(stderr, " ]\n");

    std::fprintf(stderr, "class -> %s, struct -> %s\n", m_class_name.c_str(), m_struct_name.c_str());

    std::fprintf(stderr, "%s\n", cc::repeat('*', 50).c_str());
}

void CommandOption::help(const char *name, int err_msg_code) const
{
    const static char* s_msg[] = {
        "Missing search type.",
        "Missing extension name.",
        "Unrecognize optional.",
        "Missing function signature.",
    };

    if (err_msg_code > -1) {
        std::fprintf(stderr, "Error -> %s\n", s_msg[err_msg_code]);
    }
    std::fprintf(stdout, "%s USAGE:\n", name);

    const char* help_msg = ""   \
                           "ff [type switch] [files] [switch] [option] [function signature]\n"	\
                           "\n"	\
                           "--files may be from std input \n"	\
                           "\n"	\
                           "type switch:\n"	\
                           "	-f => function,\n"	\
                           "	-d => declare\n"	\
                           "\n"	\
                           "files:\n"	\
                           "	./ -e cpp,h => directory, extension name filter\n"	\
                           "	./something.[cpp, cxx, c++] => cpp file,\n"	\
                           "	./something.[h, hpp] => cpp header,\n"	\
                           "	./something.[c] => c file\n"	\
                           "	\n"	\
                           "switch:\n"	\
                           "	-i, --ignore-case => ignore case,\n"	\
                           "	-r, --recursive => recursive find directory,\n"	\
                           "	-f, --print-filename => print file name,\n"	\
                           "	-n, --print-linenum => print line number,\n"	\
                           "	-o, --print-offset => print offset of whole file,\n"	\
                           "	-c, --print-column => print column number,\n"	\
                           "	-h, --help => print help of [ff],\n"	\
                           "	-v, --version => print version of [ff],\n"	\
                           "	-t, --only-count => only print match count, [will forbidden -n -o -c, but you can use -f]\n"	\
                           "	\n"	\
                           "option:\n"	\
                           "	-S, --start-time => start time limit of file\n"	\
                           "	[\n"	\
                           "		1449582574 -> long integer time\n"	\
                           "		2015:12:08 21:50:24 -> string format\n"	\
                           "	],\n"	\
                           "	-E, --end-time => end time of file\n"	\
                           "	[\n"	\
                           "		same as --start-time\n"	\
                           "	],\n"	\
                           "	-J, --thread => use multi thread\n"	\
                           "	[\n"	\
                           "		4 -> a integer number\n"	\
                           "	],\n"	\
                           "	-A, --clang-args => set clang's arg parse c/c++ source file\n"	\
                           "	[\n"	\
                           "		default clang's parse arg are 'c++ -x -std=c++11' \n"	\
                           "	],\n"	\
                           "	-C, --class => specify class name\n"	\
                           "	[\n"	\
                           "		class name\n"	\
                           "	],\n"	\
                           "	-T, --struct => specify struct name\n"	\
                           "	[\n"	\
                           "		struct name\n"	\
                           "	]\n"	\
                           "	\n"	\
                           "function signature sample:\n"	\
                           "	return-type name(type name, ...)  ---> full signature\n"	\
                           "		return-type name(type, ...),  ---> ignore args name\n"	\
                           "			name(type, ...),  		---> ignore return type\n"	\
                           "			void name(type, ...), 	---> specify return type as void\n"	\
                           "			return-type (type, ...), 	---> ignore function name\n"	\
                           "			return-type name(), 		---> ignore args \n"	\
                           "			return-type name(void), 	---> specify args as void\n"	\
                           "				(type, ...),			---> ignore return type and function name\n"	\
                           "				void (type, ...),   	---> ignore function name and specify return as void\n"	\
                           "				name(),				---> ignore return type and args \n"	\
                           "				name(void), 			---> ignore return type and specify args as void \n"	\
                           "				return-type (),		---> ignore function name and args \n"	\
                           "				return-type (void),	---> ignore function name and specify args as void\n"	\
                           "	name(type name, ...) -> return-type,\n"	\
                           "		name(type, ...) -> return-type,\n"	\
                           "			name(type, ...),\n"	\
                           "			name(type, ...) -> void,\n"	\
                           "			(type, ...) -> return-type,\n"	\
                           "			name() -> return-type,\n"	\
                           "			name(void) -> return-type,\n"	\
                           "				(type, ...),\n"	\
                           "				(type, ...) -> void,\n"	\
                           "				name(),\n"	\
                           "				name(void),\n"	\
                           "				() -> return-type,\n"	\
                           "				(void) -> return-type, \n"	\
                           "	return-type name(/4), ---> args number is 4\n";
    std::fprintf(stdout, "%s", help_msg);
#ifdef FF_OPTION_HELP
    std::fprintf(stdout, "%s", FF_OPTION_HELP);
#endif

    std::exit(-1);
}

time_t CommandOption::toTimet(const char *str)
{
    return std::atol(str);
}

NAMESPACE_FF_END
