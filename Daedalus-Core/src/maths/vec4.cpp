#include "ddpch.h"
#include "vec4.h"

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

	Vec4& Vec4::divide(const Vec4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	Vec4 operator+(Vec4 left, const Vec4& right)
	{
		return left.add(right);
	}

	Vec4 operator-(Vec4 left, const Vec4& right)
	{
		return left.subtract(right);
	}

	Vec4 operator*(Vec4 left, const Vec4& right)
	{
		return left.multiply(right);
	}

	Vec4 operator/(Vec4 left, const Vec4& right)
	{
		return left.divide(right);
	}

	bool Vec4::operator==(const Vec4& other)
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool Vec4::operator!=(const Vec4& other)
	{
		return !(x == other.x && y == other.y && z == other.z && w == other.w);
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

	Vec4& Vec4::operator /=(const Vec4& other)
	{
		this->divide(other);
		return *this;
	}

} }