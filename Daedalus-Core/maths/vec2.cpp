#include "vec2.h"

namespace daedalusCore { namespace maths {

	vec2::vec2() { x = 0; y = 0; }

	vec2::vec2(const float& x)
	{
		this->x = x;
		this->y = x;
	}

	vec2::vec2(const float& x, const float& y)
	{
		this->x = x;
		this->y = y;
	}

	vec2& vec2::add(const vec2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	vec2& vec2::subtract(const vec2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	vec2& vec2::multiply(const vec2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	vec2& vec2::divide(const vec2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	vec2 operator+(vec2 left, const vec2& right)
	{
		return left.add(right);
	}

	vec2 operator-(vec2 left, const vec2& right)
	{
		return left.subtract(right);
	}

	vec2 operator*(vec2 left, const vec2& right)
	{
		return left.multiply(right);
	}

	vec2 operator/(vec2 left, const vec2& right)
	{
		return left.divide(right);
	}

	bool vec2::operator==(const vec2& other)
	{
		return x == other.x && y == other.y;
	}

	bool vec2::operator!=(const vec2& other)
	{
		return !(x == other.x && y == other.y);
	}

	vec2& vec2::operator +=(const vec2& other)
	{
		this->add(other);
		return *this;
	}

	vec2& vec2::operator -=(const vec2& other)
	{
		this->subtract(other);
		return *this;
	}

	vec2& vec2::operator *=(const vec2& other)
	{
		this->multiply(other);
		return *this;
	}

	vec2& vec2::operator /=(const vec2& other)
	{
		this->divide(other);
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const vec2& vector)
	{
		stream << "vec2(" << vector.x << ", " << vector.y << ")";
		return stream;
	}
} }