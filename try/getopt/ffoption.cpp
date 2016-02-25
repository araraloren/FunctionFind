#include "ffoption.h"

#if defined(CC_PF_LINUX) || defined(CC_PF_MINGW32)
#   include <getopt.h>
#endif

#include <cstdio>
#include <iostream>
#include <cstdlib>

using namespace ff;

FFOption::FFOption()
    :m_ignore_name(false)
    ,m_ignore_ret(false)
    ,m_regex(false)
    ,m_recursion(false)
    ,m_declare(false)
    ,m_print_fn(true)
    ,m_print_ln(false)
    ,m_print_mc(false)
    ,m_start(0)
    ,m_end(0)
    ,m_signature()
    ,m_args_c(0)
    ,m_args()
    ,m_files()
{

}

//#ifdef FF_DEBUG
void
FFOption::debugPrintStatus() const
{
    std::printf("FFOption{\n");
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->m_ignore_name), this->m_ignore_name);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->m_ignore_ret), this->m_ignore_ret);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->m_regex), this->m_regex);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->m_recursion), this->m_recursion);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->m_declare), this->m_declare);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->m_print_fn), this->m_print_fn);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->m_print_ln), this->m_print_ln);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->m_print_mc), this->m_print_mc);
    std::printf("\t%s: %ld\n", FF_TO_SRTING(this->m_start), this->m_start);
    std::printf("\t%s: %ld\n", FF_TO_SRTING(this->m_end), this->m_end);
    //std::printf("\t%s: %s\n", FF_TO_SRTING(this->m_args), this->m_args.c_str());
    std::printf("\t%s: [", FF_TO_SRTING(this->m_files));
    for (std::vector<std::string>::const_iterator cit = this->m_files.begin();   \
            cit != this->m_files.end();cit ++) {
        std::printf (" %s ", cit->c_str());
    }
    std::printf("]\n");
    std::printf("\t%s:{\n", FF_TO_SRTING(this->m_signature));
    std::printf("\t\treturn type -> %s\n", this->m_signature.m_ret.c_str());
    std::printf("\t\tfunction name -> %s\n", this->m_signature.m_name.c_str());
    std::printf("\t\targment list -> %lu[", this->m_signature.m_args.size());
    for (std::vector<std::string>::const_iterator sit = this->m_signature.m_args.begin();  \
         sit != this->m_signature.m_args.end();sit ++) {
        std::printf(" %s ", sit->c_str());
    }
    std::printf("]\n\t}\n}\n");
}
//#endif

bool
FFOption::parseCommandArgs(int argc, char **argv)
{
    static const char* cmd_short_options = "R";

    static struct option cmd_long_options[] = {
        /*switch*/
		{"ignore-name",     no_argument,        0, 'N'},//0
		{"ignore-ret-type", no_argument,        0, 'Y'},//1
		{"regex",           no_argument,        0, 'G'},//2
        {"recursion",       no_argument,        0, 'R'},
        {"find-declare",    no_argument,        0, 'd'},
        {"filename",        no_argument,        0, 'f'},
        {"line-number",     no_argument,        0, 'n'},
        {"match-count",     no_argument,        0, 'c'},
        /*option*/
        {"signature",       required_argument,  0, 's'},
        {"ret-type",        required_argument,  0, 'r'},
        {"args-type",       required_argument,  0, 'a'},
        {"start-time",      required_argument,  0, 'S'},
        {"end-time",        required_argument,  0, 'E'},
        {"threads-count",   required_argument,  0, 't'},
        {"clang",           required_argument,  0,  'C'},
        /**/
        {"help",            no_argument,        0, 'h'},
        {"version",         no_argument,        0, 'v'},
        {0,                 0,                  0, 0}
    };

    std::string ret_type, args_type;

    int opt = 0;

    int option_index = 0;

    while((opt = getopt_long(argc, argv, cmd_short_options, cmd_long_options, &option_index)) != -1){
        if (opt == -1) {
            this->printOptionHelp();
            break;
        }
        switch (opt) {
        case 'N':{ this->m_ignore_name = true; } break;
        case 'Y':{ this->m_ignore_ret = true; } break;
        case 'G':{ this->m_regex = true; } break;
        case 'R':{ this->m_recursion = true; } break;
        case 'D':{ this->m_declare = true; } break;
        case 'f':{ this->m_print_fn = true; } break;
        case 'n':{ this->m_print_ln = true; } break;
            //when we set m_print_mc true
            //we need check m_print_fn
        case 'c':{ this->m_print_mc = true; } break;
        /**************************************************/
        case 's':{ this->m_signature.parseSignature(optarg); } break;
        case 'r':{ ret_type = optarg; } break;
        case 'a':{ args_type = optarg; } break;
        case 'S':{ this->setStartTime(std::atol(optarg)); } break;
        case 'E':{ this->setEndTime(std::atol(optarg)); } break;
        case 't':{ this->setThreadCount(std::atoi(optarg)); } break;
        case 'C':{ this->setClangArgs(optarg); } break;
        /*************************************************/
        case 'h':
        case 'v':
        default:{ this->printOptionHelp(); exit(0);} break;
        }
    }

    if (this->m_signature == FunctionSignature()) {
        if (!ret_type.empty()) {
            this->m_signature.parseRetType(ret_type);
        }
        if (!args_type.empty()) {
            this->m_signature.parseArgsType(args_type);
        }
    }

    if (optind >= argc) {
        return false;
    }

    while (optind < argc) {
        this->pushFiles(std::string(argv[optind ++]));
    }

    this->checkClangOptions(std::string("-x,c++,-std=c++11"));

    return true;
}

void
FFOption::printOptionHelp() const
{
    static const char* s_help = FF_OPTION_HELP;

    std::printf("%s", s_help);
}



