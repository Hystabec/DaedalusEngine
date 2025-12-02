#pragma once

#include <filesystem>

#define DD_EXPAND_MACRO(x) x
#define DD_STRINGIFY_MACRO(x) #x

#ifdef DD_DEBUG
#define DD_USING_ASSERTS
#endif

//#ifdef DD_RELEASE
//	#define DD_USING_ASSERTS
//#endif

#ifdef DD_DISTRO
#define DD_USING_VERIFY
#endif

#if defined(DD_PLATFORM_WINDOWS)
	#define DD_DEBUGBREAK() __debugbreak()
#elif defined(DD_PLATFORM_LINUX)
	#include <signal.h>
	#define DD_DEBUGBREAK() raise(SIGTRAP)
#else
	#error "Platform doesn't support debugbreak"
#endif

#ifdef DD_USING_ASSERTS
	// Modified version of Assert macros from Hazel : https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/Assert.h

	#define DD_INTERNAL_ASSERT_IMPL(type, check, msg, ...) {if(!(check)) { DD##type##LOG_ERROR(msg, __VA_ARGS__); DD_DEBUGBREAK(); }}
	#define DD_INTERNAL_ASSERT_WITH_MSG(type, check, ...) DD_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed [{} Line:{}]: {}", std::filesystem::path(__FILE__).filename().string(), __LINE__, __VA_ARGS__)
	#define DD_INTERNAL_ASSERT_NO_MSG(type, check) DD_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{}' failed [{} Line:{}]", DD_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define DD_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define DD_INTERNAL_ASSERT_GET_MACRO(...) DD_EXPAND_MACRO( DD_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, DD_INTERNAL_ASSERT_WITH_MSG, DD_INTERNAL_ASSERT_NO_MSG) )

	#define DD_ASSERT(...) DD_EXPAND_MACRO( DD_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define DD_CORE_ASSERT(...) DD_EXPAND_MACRO( DD_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )

	#define DD_ASSERT_FORMAT_MESSAGE(fmt, ...) daedalus::debug::Log::formatLogMessage(fmt, __VA_ARGS__)
#else
	#define DD_CORE_ASSERT(condition, ...)
	#define DD_ASSERT(condition, ...)

	#define DD_ASSERT_FORMAT_MESSAGE(fmt, ...)
#endif

// Verify is similar to assert but it is only active in distro builds

#ifdef DD_USING_VERIFY

	#define DD_INTERNAL_VERIFY_IMPL(type, check, msg, ...) {if(!(check)) { DD##type##LOG_ERROR(msg, __VA_ARGS__); DD_DEBUGBREAK(); }}
	#define DD_INTERNAL_VERIFY_WITH_MSG(type, check, ...) DD_INTERNAL_VERIFY_IMPL(type, check, "Assertion failed [{} Line:{}]: {}", std::filesystem::path(__FILE__).filename().string(), __LINE__, __VA_ARGS__)
	#define DD_INTERNAL_VERIFY_NO_MSG(type, check) DD_INTERNAL_VERIFY_IMPL(type, check, "Assertion '{}' failed [{} Line:{}]", DD_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define DD_INTERNAL_VERIFY_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define DD_INTERNAL_VERIFY_GET_MACRO(...) DD_EXPAND_MACRO( DD_INTERNAL_VERIFY_GET_MACRO_NAME(__VA_ARGS__, DD_INTERNAL_VERIFY_WITH_MSG, DD_INTERNAL_VERIFY_NO_MSG) )

	#define DD_VERIFY(...) DD_EXPAND_MACRO( DD_INTERNAL_VERIFY_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define DD_CORE_VERIFY(...) DD_EXPAND_MACRO( DD_INTERNAL_VERIFY_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )

	#define DD_VERIFY_FORMAT_MESSAGE(fmt, ...) daedalus::debug::Log::formatLogMessage(fmt, __VA_ARGS__)
#else
	#define DD_CORE_VERIFY(condition, ...)
	#define DD_VERIFY(condition, ...)

	#define DD_VERIFY_FORMAT_MESSAGE(fmt, ...)
#endif // DD_USING_VERIFY

