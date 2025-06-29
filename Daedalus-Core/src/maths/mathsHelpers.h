#pragma once

namespace daedalus::maths {

	constexpr auto DD_MATHS_PI = 3.14159265358979323846;

	inline float degrees_to_radians(float degrees)
	{
		return (float)(degrees * (float)DD_MATHS_PI / 180.0f);
	}

	inline float radians_to_degrees(float rads)
	{
		return (float)(rads * 180.0f / (float)DD_MATHS_PI);
	}

}