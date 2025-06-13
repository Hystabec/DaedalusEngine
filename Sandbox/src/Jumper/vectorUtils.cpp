#include "vectorUtils.h"

float jumper::vectorUtils::magnitudeVector2(const daedalusCore::maths::vec2& vector)
{
    return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}

daedalusCore::maths::vec2 jumper::vectorUtils::normalizeVector2(const daedalusCore::maths::vec2& vector)
{
    float mag = magnitudeVector2(vector);
    return { vector.x / mag, vector.y / mag };
}

float jumper::vectorUtils::magnitudeVector3(const daedalusCore::maths::vec3& vector)
{
    return sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

daedalusCore::maths::vec3 jumper::vectorUtils::normalizeVector3(const daedalusCore::maths::vec3& vector)
{
    float mag = magnitudeVector3(vector);
    return { vector.x / mag, vector.y / mag, vector.z / mag };
}
