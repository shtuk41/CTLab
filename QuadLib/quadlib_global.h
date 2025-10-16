#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QUADLIB_LIB)
#  define QUADLIB_EXPORT Q_DECL_EXPORT
# else
#  define QUADLIB_EXPORT Q_DECL_IMPORT
# endif
#else
# define QUADLIB_EXPORT
#endif
