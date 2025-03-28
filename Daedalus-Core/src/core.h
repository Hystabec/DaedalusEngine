#pragma once


#ifdef DD_PLATFORM_WINDOWS
#else
	#error Daedalus only supports windows
#endif

#ifdef DD_USING_ASSERTS
	#define DD_CORE_ASSERT(condition, ...) { if(!(condition)) { DD_CORE_LOG_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
	#define DD_ASSERT(condition, ...) { if(!(condition)) { DD_LOG_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#else
	#define DD_CORE_ASSERT(condition, ...)
	#define DD_CORE_ASSERT(condition, ...)
#endif

#define BIT(x) (1 << x)