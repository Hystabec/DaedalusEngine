#include "ddpch.h"
#include "vec4.h"

namespace daedalus { namespace maths {

	vec4::vec4() { x = 0; y = 0; z = 0; w = 0; }

	vec4::vec4(const float& x)
	{
		this->x = x;
		this->y = x;
		this->z = x;
		this->w = x;
	}

	vec4::vec4(const float& x, const float& y, const float& z, const float& w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	vec4::vec4(const vec4& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
	}

	vec4& vec4::add(const vec4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	vec4& vec4::subtract(const vec4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	vec4& vec4::multiply(const vec4& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	vec4& vec4::divide(const vec4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	vec4 operator+(vec4 left, const vec4& right)
	{
		return left.add(right);
	}

	vec4 operator-(vec4 left, const vec4& right)
	{
		return left.subtract(right);
	}

	vec4 operator*(vec4 left, const vec4& right)
	{
		return left.multiply(right);
	}

	vec4 operator/(vec4 left, const vec4& right)
	{
		return left.divide(right);
	}

	bool vec4::operator==(const vec4& other)
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool vec4::operator!=(const vec4& other)
	{
		return !(x == other.x && y == other.y && z == other.z && w == other.w);
	}

	vec4& vec4::operator +=(const vec4& other)
	{
		this->add(other);
		return *this;
	}

	vec4& vec4::operator -=(const vec4& other)
	{
		this->subtract(other);
		return *this;
	}

	vec4& vec4::operator *=(const vec4& other)
	{
		this->multiply(other);
		return *this;
	}

	vec4& vec4::operator /=(const vec4& other)
	{
		this->divide(other);
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const vec4& vector)
	{
		stream << "vec4(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
		return stream;
	}
} }