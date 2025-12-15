#pragma once

#include "constants.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

namespace daedalus::maths
{

	[[nodiscard]] constexpr float degrees_to_radians(float degrees) noexcept
	{
		return (float)(degrees * (float)DD_PI / 180.0f);
	}

	[[nodiscard]] constexpr Vec2 degrees_to_radians(const Vec2& degrees) noexcept
	{
		return Vec2(degrees_to_radians(degrees.x), degrees_to_radians(degrees.y));
	}

	[[nodiscard]] constexpr Vec3 degrees_to_radians(const Vec3& degrees) noexcept
	{
		return Vec3(degrees_to_radians(degrees.x), degrees_to_radians(degrees.y), degrees_to_radians(degrees.z));
	}

	[[nodiscard]] constexpr Vec4 degrees_to_radians(const Vec4& degrees) noexcept
	{
		return Vec4(degrees_to_radians(degrees.x), degrees_to_radians(degrees.y), degrees_to_radians(degrees.z), degrees_to_radians(degrees.w));
	}

	[[nodiscard]] constexpr float radians_to_degrees(float rads) noexcept
	{
		return (float)(rads * 180.0f / (float)DD_PI);
	}

	[[nodiscard]] constexpr Vec2 radians_to_degrees(const Vec2& rads) noexcept
	{
		return Vec2(radians_to_degrees(rads.x), radians_to_degrees(rads.y));
	}

	[[nodiscard]] constexpr Vec3 radians_to_degrees(const Vec3& rads) noexcept
	{
		return Vec3(radians_to_degrees(rads.x), radians_to_degrees(rads.y), radians_to_degrees(rads.z));
	}

	[[nodiscard]] constexpr Vec4 radians_to_degrees(const Vec4& rads) noexcept
	{
		return Vec4(radians_to_degrees(rads.x), radians_to_degrees(rads.y), radians_to_degrees(rads.z), radians_to_degrees(rads.w));
	}

}