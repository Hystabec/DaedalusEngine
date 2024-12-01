#pragma once


#ifdef DD_PLATFORM_WINDOWS
	#ifdef DD_BUILD_DLL
		#define DD_API __declspec(dllexport)
	#else
		#define DD_API __declspec(dllimport)
	#endif // DAEDALUS_BUILD_DLL
#else
	#error Daedalus only supports windows
#endif