#ifndef CCPUBLIC_H
#define CCPUBLIC_H

#include <ccconfig.h>
#include <cctypedef.h>

#include <string>
#include <vector>

#ifdef CC_PF_LINUX
#   include <unistd.h>
#   include <sys/stat.h>
#endif

#ifdef CC_PF_MINGW32
#   include <sys/stat.h>
#   include <cstdlib>
#endif

#if __cplusplus >= 201103L
#   include <stdexcept>
#   include <ostream>
#endif

using std::string;

ccNamespaceDeclare(cc)

/*
 * 睡眠函数
*/
void
microSleep(int micro_seconds);

/*
 * 格式化路径成如下
 * '.?/\z'
*/
string&
formatPath(string& path);

/*
 * 使用strftime将time转换成字符串，格式使用format给定的格式
*/
string
getLocalTimeStr(const string& format);

/*
 * 判断文件的状态
*/
#if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32)
	//#define	R_OK	4		/* Test for read permission.  */
	//#define	W_OK	2		/* Test for write permission.  */
	//#define	X_OK	1		/* Test for execute permission.  */
	//#define	F_OK	0		/* Test for existence.  */
#else
#	define	R_OK	4		/* Test for read permission.  */
#	define	W_OK	2		/* Test for write permission.  */
#	define	X_OK	1		/* Test for execute permission.  */
#	define	F_OK	0		/* Test for existence.  */
#endif

bool
accessFile(const string& file, int mode = F_OK);

/*
 * 递归的创建目录
*/
bool
createDirectory(const string& directory);


/*
 * 分割字符串
*/
std::vector<std::string>
spiltString(const std::string& str, const char sep);//sep = '|','@', etc

std::vector<std::string>
spiltString(const std::string &str, const char sep[]);//sep = {"^^"}, {"||"}, etc

std::vector<std::string>
spiltString(const std::string &str, const std::string& regex);

/*
 * 异常安全的print
*/
#if __cplusplus >= 201103L

void
printf(std::ostream& out, const char* s);

template<typename T, typename... Args>      // note the "..."
void
printf(std::ostream& out, const char* s, T value, Args... args)   // note the "..."
{
	while (*s) {
		if (*s=='%' && *++s!='%') { // a format specifier (ignore which one it is)
			out << value;     // use first non-format argument
			return printf(out, s, args...);    // "peel off" first argument
		}
		out << *s++;
	}

	throw std::runtime_error("extra arguments provided to printf");
}

/*
 * 递归的将args里面的数据输出到out里面
*/
template <typename T>
void
print(std::ostream& out, T& s)
{
	out <<s;
}

template<typename T, typename... Args>
void
print(std::ostream& out, T value, Args... args)
{
	print(out, value);
	print(out, args ...);
}

#endif

ccNamespaceEnd(cc)

#endif // CCPUBLIC_H
