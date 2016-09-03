#ifndef UTIL_GLOBAL_H
#define UTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UTIL_LIBRARY)
#  define UTILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UTILSHARED_EXPORT Q_DECL_IMPORT
#endif

#define NS_UTIL Util
#define NS_STR_UTIL "Util"

#endif // UTIL_GLOBAL_H
