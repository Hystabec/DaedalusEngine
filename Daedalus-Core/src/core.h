#pragma once


#ifdef DD_PLATFORM_WINDOWS
	#ifdef DD_BUILD_DLL
		#define DD_API __declspec(dllexport)
	#else
		#define DD_API __declspec(dllimport)
	#endif 
#else
	#error Daedalus only supports windows
#endif

#ifdef DD_USING_ASSERTS
	#define DD_CORE_ASSERT(x, ...) { if(!(x)) { CORE_LOG_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
	#define DD_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#else
	#define DD_CORE_ASSERT(x, ...)
	#define DD_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)