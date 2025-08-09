#pragma once

#include "vec2.h"
#include "mathsHelpers.h"

namespace daedalus::maths {

	struct Vec4;

	struct Vec3
	{
		union
		{
			struct { float x, y, z; };
			struct { float r, g, b; };
		};

		constexpr Vec3()
			: x(0.0f), y(0.0f), z(0.0f)
		{
		}

		constexpr Vec3(float scalar)
			: x(scalar), y(scalar), z(scalar)
		{
		}

		constexpr Vec3(float x, float y, float z)
			: x(x), y(y), z(z)
		{
		}

		constexpr Vec3(const Vec3& other)
			: x(other.x), y(other.y), z(other.z)
		{
		}

		constexpr Vec3(const Vec2& other, float z = 0.0f)
			: x(other.x), y(other.y), z(z)
		{
		}

		constexpr Vec3(const Vec4& other);

		// Locical Operators

		constexpr bool operator ==(const Vec3& other) const
		{
			return (x == other.x && y == other.y && z == other.z);
		}

		constexpr bool operator !=(const Vec3& other) const
		{
			return !(x == other.x && y == other.y && z == other.z);
		}

		// Binary Arithmetic Operators

		/// @brief Modifies caller and returns result
		constexpr Vec3& add(const Vec3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec3& subtract(const Vec3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec3& multiply(const Vec3& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec3& multiply(float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec3& divide(const Vec3& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec3& divide(float scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		friend constexpr Vec3 operator +(const Vec3& left, const Vec3& right)
		{
			return {
			left.x + right.x,
			left.y + right.y,
			left.z + right.z
			};
		}

		friend constexpr Vec3 operator -(const Vec3& left, const Vec3& right)
		{
			return {
			left.x - right.x,
			left.y - right.y,
			left.z - right.z
			};
		}

		friend constexpr Vec3 operator *(const Vec3& left, const Vec3& right)
		{
			return {
			left.x * right.x,
			left.y * right.y,
			left.z * right.z
			};
		}

		friend constexpr Vec3 operator *(const Vec3& left, float scalar)
		{
			return {
			left.x * scalar,
			left.y * scalar,
			left.z * scalar
			};
		}

		friend constexpr Vec3 operator /(const Vec3& left, const Vec3& right)
		{
			return {
			left.x / right.x,
			left.y / right.y,
			left.z / right.z
			};
		}

		friend constexpr Vec3 operator /(const Vec3& left, float scalar)
		{
			return {
			left.x / scalar,
			left.y / scalar,
			left.z / scalar
			};
		}

		// assignment operators

		constexpr Vec3& operator  =(const Vec3& other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;
		}

		constexpr Vec3& operator +=(const Vec3& other)
		{
			this->add(other);
			return *this;
		}

		constexpr Vec3& operator -=(const Vec3& other)
		{

			this->subtract(other);
			return *this;
		}

		constexpr Vec3& operator *=(const Vec3& other)
		{
			this->multiply(other);
			return *this;
		}

		constexpr Vec3& operator *=(float scalar)
		{
			this->multiply(scalar);
			return *this;
		}

		constexpr Vec3& operator /=(const Vec3& other)
		{
			this->divide(other);
			return *this;
		}

		constexpr Vec3& operator /=(float scalar)
		{
			this->divide(scalar);
			return *this;
		}

		// Unary Operators

		constexpr Vec3 operator +() const
		{
			return Vec3(+(this->x), +(this->y), +(this->z));
		}

		constexpr Vec3 operator -() const
		{
			return Vec3(-(this->x), -(this->y), -(this->z));
		}

		// Cast Operators

		constexpr operator float* () { return &x; }
		constexpr operator const float* () const { return &x; }

		// Extras / Helpers

		static inline float dot(const Vec3& left, const Vec3& right)
		{
			return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
		}

		static inline Vec3 cross(const Vec3& left, const Vec3& right)
		{
			return {
			left.y * right.z - right.y * left.z,
			left.z * right.x - right.z * left.x,
			left.x * right.y - right.x * left.y
			};
		}

		static inline float length(const Vec3& vec3)
		{
			return sqrt(Vec3::dot(vec3, vec3));
		}

		static inline Vec3 scale(const Vec3& vec3, float desiredLength)
		{
			return vec3 * desiredLength / length(vec3);
		}

	};

	constexpr Vec2::Vec2(const Vec3& other)
		: x(other.x), y(other.y)
	{
	}

}

LOG_CREATE_FORMAT(daedalus::maths::Vec3, "x: {}, y: {}, z: {}", vec, vec.x, vec.y, vec.z);