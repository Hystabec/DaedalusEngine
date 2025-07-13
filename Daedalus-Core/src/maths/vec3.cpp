#include "ddpch.h"
#include "vec3.h"

#include "vec2.h"
#include "vec4.h"

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

	Vec3::Vec3(const Vec2& other, float z)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = z;
	}

	Vec3::Vec3(const Vec4& other)
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

	float Vec3::dot(const Vec3& left, const Vec3& right)
	{
		return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
	}

	Vec3 Vec3::cross(const Vec3& left, const Vec3& right)
	{
		return {
			left.y * right.z - right.y * left.z,
			left.z * right.x - right.z * left.x,
			left.x * right.y - right.x * left.y
		};
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

	Vec3 Vec3::operator-()
	{
		return Vec3(-(this->x), -(this->y), -(this->z));
	}

} }