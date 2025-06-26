#include "ddpch.h"
#include "vec3.h"

namespace daedalus { namespace maths {

	Vec3::Vec3() { x = 0; y = 0; z = 0; }

	Vec3::Vec3(const float& x)
	{
		this->x = x;
		this->y = x;
		this->z = x;
	}

	Vec3::Vec3(const float& x, const float& y, const float& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3::Vec3(const Vec3& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	Vec3& Vec3::add(const Vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3& Vec3::subtract(const Vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vec3& Vec3::multiply(const Vec3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vec3& Vec3::divide(const Vec3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	Vec3 operator+(Vec3 left, const Vec3& right)
	{
		return left.add(right);
	}

	Vec3 operator-(Vec3 left, const Vec3& right)
	{
		return left.subtract(right);
	}

	Vec3 operator*(Vec3 left, const Vec3& right)
	{
		return left.multiply(right);
	}

	Vec3 operator/(Vec3 left, const Vec3& right)
	{
		return left.divide(right);
	}

	bool Vec3::operator==(const Vec3& other)
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool Vec3::operator!=(const Vec3& other)
	{
		return !(x == other.x && y == other.y && z == other.z);
	}

	Vec3& Vec3::operator +=(const Vec3& other)
	{
		this->add(other);
		return *this;
	}

	Vec3& Vec3::operator -=(const Vec3& other)
	{
		this->subtract(other);
		return *this;
	}

	Vec3& Vec3::operator *=(const Vec3& other)
	{
		this->multiply(other);
		return *this;
	}

	Vec3& Vec3::operator /=(const Vec3& other)
	{
		this->divide(other);
		return *this;
	}

} }