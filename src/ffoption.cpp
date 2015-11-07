#include "ffoption.h"

#if defined(CC_PF_LINUX) || defined(CC_PF_MINGW32)
#   include <getopt.h>
#endif

#include <cstdio>
#include <cstdlib>

using namespace ff;

FFOption::FFOption()
    :ignore_name_(false)
    ,ignore_ret_type_(false)
    ,regex_(false)
    ,recursion_(false)
    ,find_declare_(false)
    ,print_filename_(true)
    ,print_line_num_(false)
    ,print_match_count_(false)
    ,start_time_(0)
    ,end_time_(0)
    ,function_signature_()
{

}

//#ifdef FF_DEBUG
void
FFOption::debugPrintStatus() const
{
    std::printf("FFOption{\n");
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->ignore_name_), this->ignore_name_);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->ignore_ret_type_), this->ignore_ret_type_);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->regex_), this->regex_);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->recursion_), this->recursion_);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->find_declare_), this->find_declare_);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->print_filename_), this->print_filename_);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->print_line_num_), this->print_line_num_);
    std::printf("\t%s: %d\n", FF_TO_SRTING(this->print_match_count_), this->print_match_count_);
    std::printf("\t%s: %ld\n", FF_TO_SRTING(this->start_time_), this->start_time_);
    std::printf("\t%s: %ld\n", FF_TO_SRTING(this->end_time_), this->end_time_);
    std::printf("\t%s:{\n", FF_TO_SRTING(this->function_signature_));
    std::printf("\t\t%s\n", this->function_signature_.returnType().c_str());
    std::printf("\t\t%s\n", this->function_signature_.functionName().c_str());
    std::printf("\t\t[");
    for (std::vector<std::string>::iterator it = this->function_signature_.argsType().begin();  \
         it != this->function_signature_.argsType().end();it ++) {
        std::printf("%s", it->c_str());
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
        case 'N':{ this->ignore_name_ = true; } break;
        case 'Y':{ this->ignore_ret_type_ = true; } break;
		case 'G':{ this->regex_ = true; } break;
        case 'R':{ this->recursion_ = true; } break;
        case 'D':{ this->find_declare_ = true; } break;
        case 'f':{ this->print_filename_ = true; } break;
        case 'n':{ this->print_line_num_ = true; } break;
            //when we set print_match_count_ true
            //we need check print_filename_
        case 'c':{ this->print_match_count_ = true; } break;
        /**************************************************/
        case 's':{ this->function_signature_.parseSignature(optarg); } break;
        case 'r':{ ret_type = optarg; } break;
        case 'a':{ args_type = optarg; } break;
        case 'S':{ this->setStartTime(std::atol(optarg)); } break;
        case 'E':{ this->setEndTime(std::atol(optarg)); } break;
        case 't':{ this->setThreadCount(std::atoi(optarg)); } break;
        /*************************************************/
        case 'h':
        case 'v':
        default:{ this->printOptionHelp(); exit(0);} break;
        }
    }

    if (this->function_signature_ == FunctionSignature()) {
        if (!ret_type.empty()) {
            this->function_signature_.parseRetType(ret_type);
        }
        if (!args_type.empty()) {
            this->function_signature_.parseArgsType(args_type);
        }
    }

    return true;
}

void
FFOption::printOptionHelp() const
{
    static const char* s_help = FF_OPTION_HELP;

    std::printf(s_help);
}



