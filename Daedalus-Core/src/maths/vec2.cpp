#include "ddpch.h"
#include "vec2.h"

#include "vec3.h"
#include "vec4.h"

namespace daedalus { namespace maths {

	Vec2::Vec2() { x = 0; y = 0; }

	Vec2::Vec2(const float& x)
	{
		this->x = x;
		this->y = x;
	}

	Vec2::Vec2(const float& x, const float& y)
	{
		this->x = x;
		this->y = y;
	}

	Vec2::Vec2(const Vec2& other)
	{
		this->x = other.x;
		this->y = other.y;
	}

	Vec2::Vec2(const Vec3& other)
	{
		this->x = other.x;
		this->y = other.y;
	}

	Vec2::Vec2(const Vec4& other)
	{
		this->x = other.x;
		this->y = other.y;
	}

	bool Vec2::operator==(const Vec2& other)
	{
		return x == other.x && y == other.y;
	}

	bool Vec2::operator!=(const Vec2& other)
	{
		return !(x == other.x && y == other.y);
	}

	Vec2& Vec2::add(const Vec2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& Vec2::subtract(const Vec2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vec2& Vec2::multiply(const Vec2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vec2& Vec2::multiply(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vec2& Vec2::divide(const Vec2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Vec2& Vec2::divide(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vec2 operator+(const Vec2& left, const Vec2& right)
	{
		return {
			left.x + right.x,
			left.y + right.y
		};
	}

	Vec2 operator-(const Vec2& left, const Vec2& right)
	{
		return {
			left.x - right.x,
			left.y - right.y
		};
	}

	Vec2 operator*(const Vec2& left, const Vec2& right)
	{
		return {
			left.x * right.x,
			left.y * right.y
		};
	}

	Vec2 operator*(const Vec2& left, float scalar)
	{
		return {
			left.x * scalar,
			left.y * scalar
		};
	}

	Vec2 operator/(const Vec2& left, const Vec2& right)
	{
		return {
			left.x / right.x,
			left.y / right.y
		};
	}

	Vec2 operator/(const Vec2& left, float scalar)
	{
		return {
			left.x / scalar,
			left.y / scalar
		};
	}

	Vec2& Vec2::operator=(const Vec2& other)
	{
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vec2& Vec2::operator +=(const Vec2& other)
	{
		this->add(other);
		return *this;
	}

	Vec2& Vec2::operator -=(const Vec2& other)
	{
		this->subtract(other);
		return *this;
	}

	Vec2& Vec2::operator *=(const Vec2& other)
	{
		this->multiply(other);
		return *this;
	}

	Vec2& Vec2::operator*=(float scalar)
	{
		this->multiply(scalar);
		return *this;
	}

	Vec2& Vec2::operator /=(const Vec2& other)
	{
		this->divide(other);
		return *this;
	}

	Vec2& Vec2::operator/=(float scalar)
	{
		this->divide(scalar);
		return *this;
	}

	Vec2 Vec2::operator+() const
	{
		return Vec2(+(this->x), +(this->y));
	}

	Vec2 Vec2::operator-() const
	{
		return Vec2(-(this->x), -(this->y));
	}

	float Vec2::dot(const Vec2& left, const Vec2& right)
	{
		return (left.x * right.x) + (left.y * right.y);
	}

} }