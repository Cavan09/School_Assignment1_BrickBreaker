#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Game/GameConstants.h"

#if WIN32
#include "../../Windows/Windows Game/Win32Constants.h"
#endif

#ifdef __APPLE__
#include "OSXConstants.h"
#endif

#endif  //CONSTANTS_H
