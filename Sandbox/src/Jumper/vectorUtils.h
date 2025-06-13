#pragma once
#include <Daedalus.h>

namespace jumper::vectorUtils
{
	float magnitudeVector2(const daedalusCore::maths::vec2& vector);
	daedalusCore::maths::vec2 normalizeVector2(const daedalusCore::maths::vec2& vector);

	float magnitudeVector3(const daedalusCore::maths::vec3& vector);
	daedalusCore::maths::vec3 normalizeVector3(const daedalusCore::maths::vec3& vector);

}