#pragma once

#include <memory>
#include "platformSpecific/platformDetection.h"

#ifdef DD_PLATFORM_WINDOWS
#else
	#error Daedalus only supports windows
#endif

#ifdef DD_USING_ASSERTS
	#define DD_CORE_ASSERT(condition, ...) { if(!(condition)) { DD_CORE_LOG_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
	#define DD_ASSERT(condition, ...) { if(!(condition)) { DD_LOG_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
	#define DD_ASSERT_FORMAT_MESSAGE(fmt, ...) daedalusCore::debug::Log::formatLogMessage(fmt, __VA_ARGS__)
#else
	#define DD_CORE_ASSERT(condition, ...)
	#define DD_ASSERT(condition, ...)
	#define DD_ASSERT_FORMAT_MESSAGE(fmt, ...)
#endif

#define BIT(x) (1 << x)

namespace daedalusCore {

	template<typename T>
	using uni_ptr = std::unique_ptr<T>;

	template<typename T, typename...Args>
	constexpr uni_ptr<T> create_uni_ptr(Args&& ...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using shr_ptr = std::shared_ptr<T>;

	template<typename T, typename...Args>
	constexpr shr_ptr<T> create_shr_ptr(Args&& ...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}