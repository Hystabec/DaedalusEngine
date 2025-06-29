#pragma once

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

namespace daedalus::maths {

	constexpr double DD_MATHS_PI = 3.14159265358979323846;

	inline constexpr float degrees_to_radians(const float& degrees)
	{
		return (float)(degrees * (float)DD_MATHS_PI / 180.0f);
	}

	inline static Vec2 degrees_to_radians(const Vec2& degrees)
	{
		return Vec2{ degrees_to_radians(degrees.x), degrees_to_radians(degrees.y) };
	}

	inline static Vec3 degrees_to_radians(const Vec3& degrees)
	{
		return { degrees_to_radians(degrees.x), degrees_to_radians(degrees.y), degrees_to_radians(degrees.z) };
	}

	inline static Vec4 degrees_to_radians(const Vec4& degrees)
	{
		return { degrees_to_radians(degrees.x), degrees_to_radians(degrees.y), degrees_to_radians(degrees.z), degrees_to_radians(degrees.w) };
	}

	inline constexpr float radians_to_degrees(const float& rads)
	{
		return (float)(rads * 180.0f / (float)DD_MATHS_PI);
	}

	inline static Vec2 radians_to_degrees(const Vec2& rads)
	{
		return { radians_to_degrees(rads.x), radians_to_degrees(rads.y) };
	}

	inline static Vec3 radians_to_degrees(const Vec3& rads)
	{
		return { radians_to_degrees(rads.x), radians_to_degrees(rads.y), radians_to_degrees(rads.z) };
	}

	inline static Vec4 radians_to_degrees(const Vec4& rads)
	{
		return { radians_to_degrees(rads.x), radians_to_degrees(rads.y), radians_to_degrees(rads.z), radians_to_degrees(rads.w) };
	}
}