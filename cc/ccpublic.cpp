#include "ccpublic.h"

#ifdef CC_PF_WIN32
#	include <io.h>
#	include <direct.h>
#endif
#ifdef CC_PF_LINUX
#   include <errno.h>
#endif

#include <cctype>
#include <ctime>

#if __cplusplus >= 201103L
#   include <regex>
#endif

//copyed from https://github.com/swem/player-git-svn/blob/master/replace/nanosleep.c
#if (defined CC_PF_WIN32) || ((defined CC_PF_MINGW32) && (!defined _GLIBCXX_USE_NANOSLEEP))

struct timespec {
	time_t  tv_sec;	/* Seconds */
	long    tv_nsec;	/* Nanoseconds */
};


// Replacement for nanosleep on Windows.
// NOTES:
// The rem argument is never filled. You cannot rely on it.
// The return value will always be zero. There is no way to tell if the sleep was interrupted.
// The resolution of this is 100 nanoseconds, despite the argument being seconds and nanoseconds.
static int
s_nanosleep (const struct timespec *req, struct timespec *rem)
{
		HANDLE timer = NULL;
		LARGE_INTEGER sleepTime;

		sleepTime.QuadPart = req->tv_sec * 1000000000 + req->tv_nsec / 100;

		timer = CreateWaitableTimer (NULL, TRUE, NULL);
		if (timer == NULL)
		{
				//LPVOID buffer = NULL;
				//FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
				//                                GetLastError(), 0, (LPTSTR) &buffer, 0, NULL);
				//PLAYER_ERROR2 ("nanosleep: CreateWaitableTimer failed: (%d) %s\n",
				//                                GetLastError (), (LPTSTR) buffer);
				//LocalFree (buffer);
				return -1;
		}

		if (!SetWaitableTimer (timer, &sleepTime, 0, NULL, NULL, 0))
		{
				return -1;
		}

		if (WaitForSingleObject (timer, INFINITE) != WAIT_OBJECT_0)
		{
				return -1;
		}

		return 0;
}

static int
nanosleep(const struct timespec *req, struct timespec *rem)
{
	return s_nanosleep(rep, rem);
}

#endif

/*
 * 如果cpp为c++11，就用标准的sleep_for函数
 * 如果在linux，使用nanosleep函数
 * 如果在win32，使用自定义的nanosleep
*/
void
cc::microSleep(int micro_seconds)
{
#if __cplusplus >= 201103L
	std::this_thread::sleep_for(std::chrono::microseconds(micro_seconds));
#else//c++11
		struct timespec rep, rem;

		rep.tv_sec  = micro_seconds / 1000000;
		rem.tv_nsec = micro_seconds % 1000000;

#   if (defined CC_PF_LINUX) || ((defined CC_PF_MINGW32) && (defined _GLIBCXX_USE_NANOSLEEP))
		while(nanosleep(&rep, &rem) == -1 && errno == EINTR){
#   else
		while(nanosleep(&rep, &rem) == -1){
#   endif//nanosleep
			rep = rem;
		}
#endif//c++11
}

/*
 * 遍历path内的字符，将遇到的\\全部替换成/
 * 并且在路径的最后如不是/加上/
*/
std::string &
cc::formatPath(std::string &path)
{
	if(path.empty()){
		return path;
	}

    std::string::iterator it = path.begin();

	for(;it != path.end();it ++){
		if(*it == '\\'){
			*it = '/';
		}
	}

	if(*(it - 1) != '/'){
		path.push_back('/');
	}

	return path;
}


std::string
cc::getLocalTimeStr(const std::string &format)
{
    const size_t CONST_BUFF_LEN = 256;

    time_t      cur_time = std::time(0);
	struct tm	local_time;

#if (defined CC_PF_MINGW32) || (defined CC_PF_WIN32)
	if(localtime_s(&local_time, &cur_time) != 0){
        return string("");
	}
#elif (defined CC_PF_LINUX)
	if(localtime_r(&cur_time, &local_time) == 0){
        return string("");
	}
#endif

	char time_buff[CONST_BUFF_LEN];

	std::strftime(time_buff, CONST_BUFF_LEN - 1, format.c_str(), &local_time);

    return string(time_buff);
}


bool
cc::accessFile(const std::string &file, int mode)
{
	bool ret = true;

#ifdef CC_PF_LINUX
	if(access(file.c_str(), mode) != 0){
		return false;
	}
#elif (defined CC_PF_MINGW32)
	if(access(file.c_str(), mode) != 0){
		return false;
	}
#elif (defined CC_PF_WIN32)
	if (_access(file.c_str(), mode) != 0){
		return false;
	}
#endif

	return ret;
}


bool
cc::createDirectory(const std::string &directory)
{
	if(directory.empty()){
		return true;
	}

	bool ret = true;

    string::const_iterator last = directory.begin();

    string::const_iterator start = last;

	if(*last == '/'){
		++ last;
	}else if(*last == '.' && directory.size() >= 2){
		last += 2;
	}

    string path(directory.size(), '\0');

    string::iterator path_it = path.begin();

	for(;last != directory.end();++ last){
		if(*last == '/'){
			path_it = std::copy(start, last + 1, path_it);

#ifdef CC_PF_LINUX
			if(mkdir(path.c_str(), 0777) != 0){
				return false;
			}
#elif (defined CC_PF_MINGW32)
			if (mkdir(path.c_str()) != 0){
				return false;
			}
#elif (defined CC_PF_WIN32)
			if (_mkdir(path.c_str()) != 0){
				return false;
			}
#endif

			start = last + 1;
		}
	}

	return ret;
}

#if __cplusplus >= 201103L
void
cc::printf(std::ostream& out, const char* s)
{
	while (*s) {
		if (*s=='%' && *++s!='%')   // make sure that there wasn't meant to be more arguments
						// %% represents plain % in a format string
			 throw std::runtime_error("invalid format: missing arguments");
		out << *s++;
	}
}

#endif

std::vector<std::string>
cc::spiltString(const std::string &str, int capacity)
{
    std::vector<std::string> ret(capacity);
    std::string::const_iterator beg = str.begin();
    std::string::const_iterator cut = beg;
    std::string::const_iterator cit = beg;
    std::string::const_iterator end = str.end();

    while(cit != end) {
        for (;!std::isspace(*cit) && cit != end; cit ++)
            ;
        if (cit - cut > 0) {
            ret.insert(ret.end(), str.substr(cut - beg, cit - cut));
        }
        for (;std::isspace(*cit) && cit != end; cit ++)
            ;
        cut = cit;
    }

    return ret;
}

#include <iostream>

#if __cplusplus >= 201103L
std::vector<std::string>
cc::spiltString(const std::string &str, const std::string &sep, int capacity)
{
    std::vector<std::string> ret(capacity);
    std::regex  pattern(sep);

    std::sregex_token_iterator sit(str.begin(), str.end(), pattern, -1);
    std::sregex_token_iterator end;

    for (sit;sit != end;sit ++) {
        ret.insert(ret.end(), *sit);
    }

    return ret;
}
#endif





