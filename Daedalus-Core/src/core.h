#pragma once

#include <memory>

#ifdef DD_PLATFORM_WINDOWS
#else
	#error Daedalus only supports windows
#endif

#ifdef DD_USING_ASSERTS
	#if defined(DD_PLATFORM_WINDOWS)
		#define DD_DEBUGBREAK() __debugbreak();
	#elif defined(DD_PLATFORM_LINUX)
		#include <signal.h>
		#define DD_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak"
	#endif

	#define DD_CORE_ASSERT(condition, ...) { if(!(condition)) { DD_CORE_LOG_ERROR("Assertion Failed: {}", __VA_ARGS__); DD_DEBUGBREAK(); } }
	#define DD_ASSERT(condition, ...) { if(!(condition)) { DD_LOG_ERROR("Assertion Failed: {}", __VA_ARGS__); DD_DEBUGBREAK(); } }
	#define DD_ASSERT_FORMAT_MESSAGE(fmt, ...) daedalus::debug::Log::formatLogMessage(fmt, __VA_ARGS__)
#else
	#define DD_CORE_ASSERT(condition, ...)
	#define DD_ASSERT(condition, ...)
	#define DD_ASSERT_FORMAT_MESSAGE(fmt, ...)
#endif

#define BIT(x) (1 << x)

namespace daedalus {

	template<typename T>
	using Uni_ptr = std::unique_ptr<T>;

	template<typename T, typename...Args>
	constexpr Uni_ptr<T> create_uni_ptr(Args&& ...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Shr_ptr = std::shared_ptr<T>;

	template<typename T, typename...Args>
	constexpr Shr_ptr<T> create_shr_ptr(Args&& ...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// TO DO: Add weak ptr

}