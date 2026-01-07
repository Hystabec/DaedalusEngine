#pragma once


#ifdef DD_PLATFORM_WINDOWS
#else
	#error Daedalus only supports windows
#endif

#define BIT(x) (1 << x)

#include "ddAssert.h"
#include "ddMemory.h"

#include <memory>