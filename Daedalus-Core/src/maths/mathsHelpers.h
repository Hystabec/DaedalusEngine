#pragma once

#include <limits>

namespace daedalus::maths {

	namespace interals
	{
		template<class T, std::enable_if_t<std::is_arithmetic_v<T>>...>
		constexpr T dd_sqrtNewtonRaphson(T x, T curr, T prev)
		{
			return curr == prev ? curr : dd_sqrtNewtonRaphson(x, T(0.5) * (curr + x / curr), curr);
		}

		constexpr float dd_sqrtNewtonRaphson(int x, int curr, int prev)
		{
			return curr == prev ? curr : dd_sqrtNewtonRaphson((float)x, (0.5f * (curr + (float)x / (float)curr)), (float)curr);
		}
	}

	/// @brief Returns nan if a invalid input is given
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>...>
	constexpr T dd_sqrt(T x)
	{
		if (x >= T(0) && x < std::numeric_limits<T>::infinity())
			return interals::dd_sqrtNewtonRaphson(x, x, T(0));
		else
		{
			DD_CORE_LOG_ERROR("dd_sqrt invalid input: {}", x);
			return std::numeric_limits<T>::quiet_NaN();
		}
	}

	/// @brief Returns 0 if an invalid input is given
	constexpr float dd_sqrt(int x)
	{
		if (x >= 0 && x < INT32_MAX)
			return interals::dd_sqrtNewtonRaphson(x, x, 0);
		else
		{
			DD_CORE_LOG_ERROR("dd_sqrt invalid input: {}", x);
			return 0;
		}
	}

	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>...>
	constexpr T dd_abs(const T& x)
	{
		return x < T(0) ? -x : x;
	}

	constexpr bool epsilon_equal(float x, float y)
	{
		return dd_abs(x - y) < std::numeric_limits<float>::epsilon();
	}

	constexpr bool epsilon_not_equal(float x, float y)
	{
		return dd_abs(x - y) >= std::numeric_limits<float>::epsilon();
	}

}