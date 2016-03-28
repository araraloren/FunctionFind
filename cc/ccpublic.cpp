#include "ccpublic.h"

#ifdef CC_PF_WIN32
#	include <io.h>
#	include <direct.h>
#endif
#ifdef CC_PF_LINUX
#   include <errno.h>
#   include <dirent.h>
#   include <sys/stat.h>
#endif

#include <cstring>
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
cc::accessFile(const char *file, int mode)
{
    bool ret = true;

#ifdef CC_PF_LINUX
    if(access(file, mode) != 0){
        return false;
    }
#elif (defined CC_PF_MINGW32)
    if(access(file, mode) != 0){
        return false;
    }
#elif (defined CC_PF_WIN32)
    if (_access(file, mode) != 0){
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

std::string
cc::repeat(char ch, size_t count)
{
    string ret;

    for (size_t i = 1;i < count;i ++) {
        ret.push_back(ch);
    }

    return ret;
}

std::string
cc::repeat(const char* str, size_t count)
{
    string ret;

    for (size_t i = 1;i < count;i ++) {
        ret += str;
    }

    return ret;
}

std::string
cc::repeat(const string& str, size_t count)
{
    string ret;

    for (size_t i = 1;i < count;i ++) {
        ret += str;
    }

    return ret;
}

std::vector<std::string>
cc::splitString(const std::string &str, int capacity)
{
    std::vector<std::string> ret;
    std::string::const_iterator beg = str.begin();
    std::string::const_iterator cut = beg;
    std::string::const_iterator cit = beg;
    std::string::const_iterator end = str.end();

    ret.reserve(capacity);
    while(cit != end) {
        for (;!std::isspace(*cit) && cit != end; cit ++)
            ;
        if (cit - cut > 0) {
            ret.push_back(str.substr(cut - beg, cit - cut));
        }
        for (;std::isspace(*cit) && cit != end; cit ++)
            ;
        cut = cit;
    }

    return ret;
}

std::vector<std::string>
cc::splitString(const std::string &str, cc::format_func *pf, int capacity)
{
    std::vector<std::string> ret;
    std::string::const_iterator beg = str.begin();
    std::string::const_iterator cut = beg;
    std::string::const_iterator cit = beg;
    std::string::const_iterator end = str.end();

    ret.reserve(capacity);
    while(cit != end) {
        for (;!std::isspace(*cit) && cit != end; cit ++)
            ;
        if (cit - cut > 0) {
            ret.push_back(pf(str.substr(cut - beg, cit - cut)));
        }
        for (;std::isspace(*cit) && cit != end; cit ++)
            ;
        cut = cit;
    }

    return ret;
}

#ifdef CC_REGEX
std::vector<std::string>
cc::splitString(const std::string &str, const std::string &sep, int capacity)
{
    std::vector<std::string> ret;
    std::regex  pattern(sep);
    std::sregex_token_iterator sit(str.begin(), str.end(), pattern, -1);
    std::sregex_token_iterator end;

    for (ret.reserve(capacity);sit != end;sit ++) {
        ret.push_back(*sit);
    }

    return ret;
}

std::vector<std::string>
cc::splitString(const std::string &str, const std::string &sep, cc::format_func *pf,int capacity)
{
    std::vector<std::string> ret;
    std::regex  pattern(sep);
    std::sregex_token_iterator sit(str.begin(), str.end(), pattern, -1);
    std::sregex_token_iterator end;

    for (ret.reserve(capacity);sit != end;sit ++) {
        ret.push_back(pf(*sit));
    }

    return ret;
}
#endif

std::string
cc::trim(const std::string &str)
{//may be implement by a static whitespace table
    std::string::size_type beg = 0;
    std::string::size_type end = str.length() - 1;

    while(beg <= end) {
        if (std::isspace(str[beg])) {
            ++ beg;
        } else {
            break;
        }
    }

    while(beg <= end) {
        if (std::isspace(str[end])) {
            -- end;
        } else {
            break;
        }
    }

    return str.substr(beg, end - beg + 1);
}


const char *
cc::getExtname(const char *filename)
{
    if (!filename) return nullptr;

    const char* pext = nullptr;

    if (!(pext = std::strrchr(filename, '.'))) {
        return nullptr;
    }
    else {
        return pext++;
    }
}

const char *
cc::getExtname(const char *filename, size_t len)
{
    if (!filename || !len) return nullptr;

    const char* pext = nullptr;

    for (;len > 0;len --) {
        if (*(pext + len - 1) == '.') {
            return pext + len - 1;
        }
    }

    return nullptr;
}

std::string
cc::getExtname(const std::string &filename)
{
    std::string::size_type pos = 0;

    pos = filename.rfind('.');
    if (pos == std::string::npos) {
        return std::string();
    }
    else {
        return filename.substr(pos + 1);
    }
}

bool
cc::isDirectory(const char *path)
{
    struct stat fstat;

    if (-1 != lstat(path, &fstat)) {
        return S_ISDIR(fstat.st_mode);
    }

    return false;
}

bool
cc::isDirectory(const std::string& path)
{
    struct stat fstat;

    if (-1 != lstat(path.c_str(), &fstat)) {
        return S_ISDIR(fstat.st_mode);
    }

    return false;
}

// ############################# for search directory
namespace {

const static size_t SEARCH_PATH_LENGTH = 1024;

using std::strcmp;

    // extname -> "ext" or ""(no extname) or { "x", "y", 0 }
    void
    searchDirectoryMultiExtname(const char* directory, std::vector<std::string>& files, bool subdir, const std::vector<std::string>& exts, bool no_extname)
    {
        DIR* pdir = nullptr;

        if ((pdir = opendir(directory)) == nullptr) {
            return;
        }

        struct dirent entry;
        struct dirent *res;

        struct stat fstat;

        const char* name;

        char curpath[SEARCH_PATH_LENGTH];

        while (readdir_r(pdir, &entry, &res) == 0 && res) {
            name = entry.d_name;

            if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
                continue;
            }

            int count = std::snprintf(curpath, SEARCH_PATH_LENGTH, "%s", directory);

            count += std::snprintf(&curpath[count], SEARCH_PATH_LENGTH - count, "%s%s", \
                                   curpath[count - 1] == '/' ? "" : "/", name);

            if (lstat(curpath, &fstat) != -1) {
                if (!S_ISDIR(fstat.st_mode)) {
                    const char *e = std::strrchr(curpath, '.');

                    e++;
                    if (e != nullptr) {
                        for (std::vector<std::string>::const_iterator ci = exts.begin();
                             ci != exts.end(); ci ++) {
                            if (strcmp(e, ci->c_str()) == 0) {
                                files.push_back(std::string(curpath, count));
                                break;
                            }
                        }
                    } else if ((e == nullptr && no_extname)) {
                        files.push_back(std::string(curpath, count));
                    }
                } else if (subdir) {
                    searchDirectoryMultiExtname(curpath, files, subdir, exts, no_extname);
                }
            }
        }

        closedir(pdir);
    }

    void
    searchDirectoryExtname(const char* directory, std::vector<std::string>& files, bool subdir, const char*  extname)
    {
        DIR* pdir = nullptr;

        if ((pdir = opendir(directory)) == nullptr) {
            return;
        }

        struct dirent entry;
        struct dirent *res;

        struct stat fstat;

        const char* name;

        char curpath[SEARCH_PATH_LENGTH];

        while (readdir_r(pdir, &entry, &res) == 0 && res) {
            name = entry.d_name;

            if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
                continue;
            }

            int count = std::snprintf(curpath, SEARCH_PATH_LENGTH, "%s", directory);

            count += std::snprintf(&curpath[count], SEARCH_PATH_LENGTH - count, "%s%s", \
                                   curpath[count - 1] == '/' ? "" : "/", name);

            if (lstat(curpath, &fstat) != -1) {
                if (!S_ISDIR(fstat.st_mode)) {
                    const char *e = std::strrchr(curpath, '.');

                    e++;
                    if (e != nullptr && strcmp(e + 1, extname) == 0) {
                        files.push_back(std::string(curpath, count));
                    }
                } else if (subdir) {
                    searchDirectoryExtname(curpath, files, subdir, extname);
                }
            }
        }

        closedir(pdir);
    }

    void
    searchDirectoryNoExtname(const char* directory, std::vector<std::string>& files, bool subdir)
    {
        DIR* pdir = nullptr;

        if ((pdir = opendir(directory)) == nullptr) {
            return;
        }

        struct dirent entry;
        struct dirent *res;

        struct stat fstat;

        const char* name;

        char curpath[SEARCH_PATH_LENGTH];

        while (readdir_r(pdir, &entry, &res) == 0 && res) {
            name = entry.d_name;

            if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
                continue;
            }

            int count = std::snprintf(curpath, SEARCH_PATH_LENGTH, "%s", directory);

            count += std::snprintf(&curpath[count], SEARCH_PATH_LENGTH - count, "%s%s", \
                                   curpath[count - 1] == '/' ? "" : "/", name);

            if (lstat(curpath, &fstat) != -1) {
                if (!S_ISDIR(fstat.st_mode)) {
                    const char *e = std::strrchr(curpath, '.');

                    e++;
                    if (e == nullptr) {
                        files.push_back(std::string(curpath, count));
                    }
                } else if (subdir) {
                    searchDirectoryNoExtname(curpath, files, subdir);
                }
            }
        }

        closedir(pdir);
    }

    void
    searchDirectoryAllExtname(const char* directory, std::vector<std::string>& files, bool subdir)
    {
        DIR* pdir = nullptr;

        if ((pdir = opendir(directory)) == nullptr) {
            return;
        }

        struct dirent entry;
        struct dirent *res;

        struct stat fstat;

        const char* name;

        char curpath[SEARCH_PATH_LENGTH];

        while (readdir_r(pdir, &entry, &res) == 0 && res) {
            name = entry.d_name;

            if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
                continue;
            }

            int count = std::snprintf(curpath, SEARCH_PATH_LENGTH, "%s", directory);

            count += std::snprintf(&curpath[count], SEARCH_PATH_LENGTH - count, "%s%s", \
                                   curpath[count - 1] == '/' ? "" : "/", name);

            if (lstat(curpath, &fstat) != -1) {
                if (!S_ISDIR(fstat.st_mode)) {
                    files.push_back(std::string(curpath, count));
                } else if (subdir) {
                    searchDirectoryAllExtname(curpath, files, subdir);
                }
            }
        }

        closedir(pdir);
    }
}

std::vector<std::string>
cc::searchDirectory(const std::string &directory)
{
    std::vector<std::string> files;

    searchDirectoryAllExtname(directory.c_str(), files, false);

    return files;
}

std::vector<std::string>
cc::searchDirectory(const std::string &directory, bool recursive)
{
    std::vector<std::string> files;

    searchDirectoryAllExtname(directory.c_str(), files, recursive);

    return files;
}

std::vector<std::string>
cc::searchDirectory(const std::string &directory, const char *extname)
{
    std::vector<std::string> files;

    switch (*extname) {
    case '\0': searchDirectoryNoExtname(directory.c_str(), files, false); break;
    case '*' : searchDirectoryAllExtname(directory.c_str(), files, false); break;
    default  : searchDirectoryExtname(directory.c_str(), files, false, extname); break;
    }

    return files;
}

std::vector<std::string>
cc::searchDirectory(const std::string &directory, bool recursive, const char *extname)
{
    std::vector<std::string> files;

    switch (*extname) {
    case '\0': searchDirectoryNoExtname(directory.c_str(), files, recursive); break;
    case '*' : searchDirectoryAllExtname(directory.c_str(), files, recursive); break;
    default  : searchDirectoryExtname(directory.c_str(), files, recursive, extname); break;
    }

    return files;
}

std::vector<std::string>
cc::searchDirectory(const char *directory, bool recursive, const char *extname)
{
    std::vector<std::string> files;

    switch (*extname) {
    case '\0': searchDirectoryNoExtname(directory, files, recursive); break;
    case '*' : searchDirectoryAllExtname(directory, files, recursive); break;
    default  : searchDirectoryExtname(directory, files, recursive, extname); break;
    }

    return files;
}

std::vector<std::string>
cc::searchDirectory(const std::string &directory, bool recursive, const std::vector<std::string>& exts)
{
    std::vector<std::string> files, _exts;

    bool no_extname =  false;

    for (std::vector<std::string>::const_iterator it = exts.begin();    \
         it != exts.end();it ++) {
        if (!it->empty()) {
            _exts.push_back(*it);
        }
        no_extname = true;
    }

    searchDirectoryMultiExtname(directory.c_str(), files, recursive, _exts, no_extname);

    return files;
}

std::string
cc::toString(bool boolean)
{
    return string(boolean ? "true" : "false");
}
