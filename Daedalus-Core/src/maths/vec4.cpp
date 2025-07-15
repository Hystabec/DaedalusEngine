#include "ddpch.h"
#include "vec4.h"

#include "vec2.h"
#include "vec3.h"

namespace daedalus { namespace maths {

	Vec4::Vec4() { x = 0; y = 0; z = 0; w = 0; }

	Vec4::Vec4(const float& x)
	{
		this->x = x;
		this->y = x;
		this->z = x;
		this->w = x;
	}

	Vec4::Vec4(const float& x, const float& y, const float& z, const float& w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vec4::Vec4(const Vec4& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
	}

	Vec4::Vec4(const Vec2& other, float z, float w)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = z;
		this->w = w;
	}

	Vec4::Vec4(const Vec3& other, float w)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = w;
	}

	bool Vec4::operator==(const Vec4& other)
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool Vec4::operator!=(const Vec4& other)
	{
		return !(x == other.x && y == other.y && z == other.z && w == other.w);
	}

	Vec4& Vec4::add(const Vec4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	Vec4& Vec4::subtract(const Vec4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	Vec4& Vec4::multiply(const Vec4& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	Vec4& Vec4::multiply(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	Vec4& Vec4::divide(const Vec4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	Vec4& Vec4::divide(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	Vec4 operator+(const Vec4& left, const Vec4& right)
	{
		return {
			left.x + right.x,
			left.y + right.y,
			left.z + right.z,
			left.w + right.w
		};
	}

	Vec4 operator-(const Vec4& left, const Vec4& right)
	{
		return {
			left.x - right.x,
			left.y - right.y,
			left.z - right.z,
			left.w - right.w
		};
	}

	Vec4 operator*(const Vec4& left, const Vec4& right)
	{
		return {
			left.x * right.x,
			left.y * right.y,
			left.z * right.z,
			left.w * right.w
		};
	}

	Vec4 operator*(const Vec4& left, float scalar)
	{
		return {
			left.x * scalar,
			left.y * scalar,
			left.z * scalar,
			left.w * scalar
		};
	}

	Vec4 operator/(const Vec4& left, const Vec4& right)
	{
		return {
			left.x / right.x,
			left.y / right.y,
			left.z / right.z,
			left.w / right.w
		};
	}

	Vec4 operator/(const Vec4& left, float scalar)
	{
		return {
			left.x / scalar,
			left.y / scalar,
			left.z / scalar,
			left.w / scalar
		};
	}

	Vec4& Vec4::operator +=(const Vec4& other)
	{
		this->add(other);
		return *this;
	}

	Vec4& Vec4::operator -=(const Vec4& other)
	{
		this->subtract(other);
		return *this;
	}

	Vec4& Vec4::operator *=(const Vec4& other)
	{
		this->multiply(other);
		return *this;
	}

	Vec4& Vec4::operator *=(float scalar)
	{
		this->multiply(scalar);
		return *this;
	}

	Vec4& Vec4::operator /=(const Vec4& other)
	{
		this->divide(other);
		return *this;
	}

	Vec4& Vec4::operator /=(float scalar)
	{
		this->divide(scalar);
		return *this;
	}

	Vec4 Vec4::operator+() const
	{
		return Vec4(+(this->x), +(this->y), +(this->z), +(this->w));
	}

	Vec4 Vec4::operator-() const
	{
		return Vec4(-(this->x), -(this->y), -(this->z), -(this->w));
	}

	float Vec4::dot(const Vec4& left, const Vec4& right)
	{
		return (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w);
	}

} }