#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

namespace daedalus { namespace maths {

	inline float degrees_to_radians(float degrees)
	{
		return (float)(degrees * (float)M_PI / 180.0f);
	}

	inline float radians_to_degrees(float rads)
	{
		return (float)(rads * 180.0f / (float)M_PI);
	}

} }