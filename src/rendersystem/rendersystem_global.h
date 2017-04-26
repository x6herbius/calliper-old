#ifndef RENDERSYSTEM_GLOBAL_H
#define RENDERSYSTEM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RENDERSYSTEM_LIBRARY)
#  define RENDERSYSTEMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RENDERSYSTEMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace RenderSystem
{
    extern const char* STR_NAMESPACE;
}

#endif // RENDERSYSTEM_GLOBAL_H
