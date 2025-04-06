#pragma once

//File from Hazel - https://github.com/TheCherno/Hazel/tree/master

// Platform detection using predefined macros

#ifdef _WIN32
	#ifdef _WIN64 // windows x64
		#define DD_PLATFORM_WINDOWS
	#else // windows x86
		#error "x86 Builds are not supported"
	#endif

#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define DD_PLATFORM_IOS
	#elif TARGET_OS_MAC == 1
		#define DD_PLATFORM_MACOS
	#else
		#error "Unknown Apple platform"
	#endif

/* We also have to check __ANDROID__ before __linux__
* since android is based on the linux kernel
* it has __linux__ defined */

#elif defined(__ANDRIOD__)
	#define DD_PLATFORM_ANDRIOD

#elif defined(__linux__)
	#define DD_PLATFORM_LINUX

#else
	#error "Unknown platform"
#endif