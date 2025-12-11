#pragma once

#include <limits>

namespace daedalus::maths {

	namespace interals
	{
		template<class T, std::enable_if_t<std::is_arithmetic_v<T>>...>
		[[nodiscard]] constexpr T sqrtNewtonRaphson(T x, T curr, T prev) noexcept
		{
			return curr == prev ? curr : sqrtNewtonRaphson(x, T(0.5) * (curr + x / curr), curr);
		}

		[[nodiscard]] constexpr float sqrtNewtonRaphson(int x, int curr, int prev) noexcept
		{
			return curr == prev ? curr : sqrtNewtonRaphson((float)x, (0.5f * (curr + (float)x / (float)curr)), (float)curr);
		}
	}

	/// @brief Returns nan if a invalid input is given
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>...>
	[[nodiscard]] constexpr T sqrt(T x) noexcept
	{
		if (x >= T(0) && x < std::numeric_limits<T>::infinity())
			return interals::sqrtNewtonRaphson(x, x, T(0));
		else
		{
			DD_CORE_LOG_ERROR("sqrt invalid input: {}", x);
			return std::numeric_limits<T>::quiet_NaN();
		}
	}

	/// @brief Returns 0 if an invalid input is given
	[[nodiscard]] constexpr float sqrt(int x) noexcept
	{
		if (x >= 0 && x < INT32_MAX)
			return interals::sqrtNewtonRaphson(x, x, 0);
		else
		{
			DD_CORE_LOG_ERROR("sqrt invalid input: {}", x);
			return 0;
		}
	}

	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>...>
	[[nodiscard]] constexpr T abs(const T& x) noexcept
	{
		return x < T(0) ? -x : x;
	}

	[[nodiscard]] constexpr bool epsilon_equal(float x, float y) noexcept
	{
		return abs(x - y) < std::numeric_limits<float>::epsilon();
	}

	[[nodiscard]] constexpr bool epsilon_not_equal(float x, float y) noexcept
	{
		return abs(x - y) >= std::numeric_limits<float>::epsilon();
	}

	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>>
	[[nodiscard]] constexpr const T& min(const T& left, const T& right) noexcept
	{
		return left > right ? right : left;
	};

	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>...>
	[[nodiscard]] constexpr const T& max(const T& left, const T& right) noexcept
	{
		return left > right ? left : right;
	};

}