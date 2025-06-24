#pragma once

#include "maths/vec4.h"

namespace daedalusCore { namespace utils {

	/*static maths::vec4 colour_uint_to_vec4(uint32_t colour)
	{
		int r = colour(0 << 7) |= 255;
		int g = colour & (8 << 15);
		int b = colour & (16 << 23);
		int a = colour & (24 << 31);

		return maths::vec4(r / 255, g / 255, b / 255, a / 255);
	}*/

	static uint32_t colour_vec4_to_uint(const maths::vec4& colour)
	{
		int r = (int)(colour.x * 255);
		int g = (int)(colour.y * 255);
		int b = (int)(colour.z * 255);
		int a = (int)(colour.w * 255);
		return (a << 24 | b << 16 | g << 8 | r);
	}

	static maths::vec4 colour_vec4_to_normalized_vec4(const maths::vec4& colour)
	{
		return maths::vec4(colour.x / 255, colour.y / 255, colour.z / 255, colour.w / 255);
	}

} }