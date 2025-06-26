#include "ddpch.h"
#include "vec2.h"

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

	Vec2& Vec2::divide(const Vec2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Vec2 operator+(Vec2 left, const Vec2& right)
	{
		return left.add(right);
	}

	Vec2 operator-(Vec2 left, const Vec2& right)
	{
		return left.subtract(right);
	}

	Vec2 operator*(Vec2 left, const Vec2& right)
	{
		return left.multiply(right);
	}

	Vec2 operator/(Vec2 left, const Vec2& right)
	{
		return left.divide(right);
	}

	bool Vec2::operator==(const Vec2& other)
	{
		return x == other.x && y == other.y;
	}

	bool Vec2::operator!=(const Vec2& other)
	{
		return !(x == other.x && y == other.y);
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

	Vec2& Vec2::operator /=(const Vec2& other)
	{
		this->divide(other);
		return *this;
	}

} }