#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QUADCONTROL_LIB)
#  define QUADCONTROL_EXPORT Q_DECL_EXPORT
# else
#  define QUADCONTROL_EXPORT Q_DECL_IMPORT
# endif
#else
# define QUADCONTROL_EXPORT
#endif
