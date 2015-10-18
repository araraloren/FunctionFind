#ifndef FFCONFIG_H
#define FFCONFIG_H

#include <ccconfig.h>

#define FF_VERSION   "0.0.1"

#define FF_OPTION_HELP  \
    "functionfind version " FF_VERSION "\n"    \
    "create by lokalafina\n"    \
    "create time " __DATE__ "\n"

#define FF_TO_SRTING(x) #x

#ifdef CC_DEBUG
#   define FF_DEBUG
#endif

#define NAMESPACE_FF_BEGIN namespace ff{

#define NAMESPACE_FF_END    }

#define FF_AVOID_WARNING(x) (void)x

#endif // CONFIG_H

