#include "ddpch.h"
#include "vec3.h"

namespace daedalusCore { namespace maths {

	vec3::vec3() { x = 0; y = 0; z = 0; }

	vec3::vec3(const float& x)
	{
		this->x = x;
		this->y = x;
		this->z = x;
	}

	vec3::vec3(const float& x, const float& y, const float& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	vec3& vec3::add(const vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	vec3& vec3::subtract(const vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	vec3& vec3::multiply(const vec3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	vec3& vec3::divide(const vec3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	vec3 operator+(vec3 left, const vec3& right)
	{
		return left.add(right);
	}

	vec3 operator-(vec3 left, const vec3& right)
	{
		return left.subtract(right);
	}

	vec3 operator*(vec3 left, const vec3& right)
	{
		return left.multiply(right);
	}

	vec3 operator/(vec3 left, const vec3& right)
	{
		return left.divide(right);
	}

	bool vec3::operator==(const vec3& other)
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool vec3::operator!=(const vec3& other)
	{
		return !(x == other.x && y == other.y && z == other.z);
	}

	vec3& vec3::operator +=(const vec3& other)
	{
		this->add(other);
		return *this;
	}

	vec3& vec3::operator -=(const vec3& other)
	{
		this->subtract(other);
		return *this;
	}

	vec3& vec3::operator *=(const vec3& other)
	{
		this->multiply(other);
		return *this;
	}

	vec3& vec3::operator /=(const vec3& other)
	{
		this->divide(other);
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const vec3& vector)
	{
		stream << "vec3(" << vector.x << ", " << vector.y << ", " << vector.z <<  ")";
		return stream;
	}
} }