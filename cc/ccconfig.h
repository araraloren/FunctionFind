#ifndef CCCONFIG_H
#define CCCONFIG_H

//platform
#ifdef __linux__
#   define CC_PF_LINUX
#elif (defined __MINGW32__)
#   define CC_PF_MINGW32
#elif (defined __CYGWIN__)
#   define CC_PF_CYGWIN
#elif ((defined _WIN32) && (! defined _MFC_VER))
#   define CC_PF_WIN32
#elif (defined _MFC_VER)
#   define CC_PF_MFC
#else
#   error "not support ..."
#endif


//compiler
#if (defined __GNUC__) && (!defined __clang__)
#   define CC_C_GCC
#elif (defined _MSC_VER)
#   define CC_C_MSVC
#elif (defined __clang__)
#   define CC_C_CLANG
#else
#   error "not support ..."
#endif


//remove clang register warning
#ifdef CC_C_CLANG
#   define register
#endif

#if __cplusplus >= 201103L
#   ifdef CC_C_GCC
#       if __GNUC__ > 4 && __GNUC_MINOR__ >= 9 || __GNUC__ >= 5
#           define CC_REGEX
#       endif
#   elif (defined CC_C_CLANG)
#       define CC_REGEX
#   endif
#endif

#if (defined CC_PF_WIN32) || (defined CC_PF_MFC)
#	ifdef _WIN64
#		define __SIZEOF_POINTER__ 8
#else
#		define __SIZEOF_POINTER__ 4
#	endif
#endif

/*
 * 32bit 和 64bit系统根据系统的指针大小来判断
*/  //32ILP 64LP
#if __SIZEOF_POINTER__ == 4
#   define  CC_PF_32BIT
#elif __SIZEOF_POINTER__ == 8
#   define  CC_PF_64BIT
#endif


/*
 * win32的线程函数头和linux下的函数头稍微不一样
 * The thread fucntion head are different between win32 and linux
*/
#ifndef CC_THREAD
#   if __cplusplus >= 201103L
#       define CC_THREAD
#   else
#       ifdef CC_PF_LINUX
#           define CC_THREAD
#       elif (defined CC_PF_WIN32)
#           define CC_THREAD __stdcall
#       else
#           define CC_THREAD
#       endif
#   endif
#endif

/*
 * namespace
*/
#define ccNamespaceDeclare(x)	\
    namespace x {

#define ccNamespaceEnd(x)	\
}

/*
 * 类型注册宏
*/
#ifdef ccTypeRegister
#   undef ccTypeRegister
#endif

#define ccTypeRegister(cc_alias, type) \
		typede##f type cc_alias


/*
 * 编译时使用-Wextra -Wall（对于gcc）
 * 使用下面的宏定义取消未使用警告
*/
#define CC_VOID_CAST(x) \
		(void)x

//declare cc namespace
ccNamespaceDeclare(cc)
ccNamespaceEnd(cc)

#endif // CONFIG_H

