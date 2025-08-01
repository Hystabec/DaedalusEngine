#pragma once


#ifdef DD_PLATFORM_WINDOWS
#else
	#error Daedalus only supports windows
#endif

#include "ddAssert.h"
#include <memory>

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