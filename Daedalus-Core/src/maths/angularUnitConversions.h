#pragma once

#include "constants.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

namespace daedalus::maths
{

	constexpr float degrees_to_radians(float degrees)
	{
		return (float)(degrees * (float)DD_PI / 180.0f);
	}

	constexpr Vec2 degrees_to_radians(const Vec2& degrees)
	{
		return Vec2(degrees_to_radians(degrees.x), degrees_to_radians(degrees.y));
	}

	constexpr Vec3 degrees_to_radians(const Vec3& degrees)
	{
		return Vec3(degrees_to_radians(degrees.x), degrees_to_radians(degrees.y), degrees_to_radians(degrees.z));
	}

	constexpr Vec4 degrees_to_radians(const Vec4& degrees)
	{
		return Vec4(degrees_to_radians(degrees.x), degrees_to_radians(degrees.y), degrees_to_radians(degrees.z), degrees_to_radians(degrees.w));
	}

	constexpr float radians_to_degrees(float rads)
	{
		return (float)(rads * 180.0f / (float)DD_PI);
	}

	constexpr Vec2 radians_to_degrees(const Vec2& rads)
	{
		return Vec2(radians_to_degrees(rads.x), radians_to_degrees(rads.y));
	}

	constexpr Vec3 radians_to_degrees(const Vec3& rads)
	{
		return Vec3(radians_to_degrees(rads.x), radians_to_degrees(rads.y), radians_to_degrees(rads.z));
	}

	constexpr Vec4 radians_to_degrees(const Vec4& rads)
	{
		return Vec4(radians_to_degrees(rads.x), radians_to_degrees(rads.y), radians_to_degrees(rads.z), radians_to_degrees(rads.w));
	}

}