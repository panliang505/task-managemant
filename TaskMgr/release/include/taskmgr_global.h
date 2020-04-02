#ifndef TASKMGR_GLOBAL_H
#define TASKMGR_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef TASKMGR_LIB
# define TASKMGR_EXPORT Q_DECL_EXPORT
#else
# define TASKMGR_EXPORT Q_DECL_IMPORT
#endif

#endif // TASKMGR_GLOBAL_H
