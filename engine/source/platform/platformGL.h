//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PLATFORMGL_H_
// redirect to appropriate platform file
#if defined(TORQUE_OS_OSX)
#include "platformOSX/platformGL.h"
#elif defined(TORQUE_OS_WIN32)
#include "platformWin32/platformGL.h"
#elif defined(TORQUE_OS_LINUX) || defined(TORQUE_OS_OPENBSD) || defined(TORQUE_OS_FREEBSD)
#include "platformX86UNIX/platformGL.h"
#elif defined(TORQUE_OS_IOS)
#include "platformiOS/platformGL.h"
#endif
#endif
