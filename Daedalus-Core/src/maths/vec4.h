#pragma once

#include "vec2.h"
#include "vec3.h"

namespace daedalus::maths {

	struct Vec4
	{
		union
		{
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
		};


		constexpr Vec4()
			: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{
		}

		constexpr Vec4(float scalar)
			: x(scalar), y(scalar), z(scalar), w(scalar)
		{
		}

		constexpr Vec4(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w)
		{
		}

		constexpr Vec4(const Vec4& other)
			: x(other.x), y(other.y), z(other.z), w(other.w)
		{
		}

		constexpr Vec4(const Vec2& other, float z = 0.0f, float w = 0.0f)
			: x(other.x), y(other.y), z(z), w(w)
		{
		}

		constexpr Vec4(const Vec3& other, float w = 0.0f)
			: x(other.x), y(other.y), z(other.z), w(w)
		{
		}

		// Locical Operators

		constexpr bool operator ==(const Vec4& other) const
		{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}

		constexpr bool operator !=(const Vec4& other) const
		{
			return !(x == other.x && y == other.y && z == other.z && w == other.w);
		}

		// Binary Arithmetic Operators

		/// @brief Modifies caller and returns result
		constexpr Vec4& add(const Vec4& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec4& subtract(const Vec4& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec4& multiply(const Vec4& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec4& multiply(float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec4& divide(const Vec4& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec4& divide(float scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
			return *this;
		}

		friend constexpr Vec4 operator +(const Vec4& left, const Vec4& right)
		{
			return {
			left.x + right.x,
			left.y + right.y,
			left.z + right.z,
			left.w + right.w
			};
		}

		friend constexpr Vec4 operator -(const Vec4& left, const Vec4& right)
		{
			return {
			left.x - right.x,
			left.y - right.y,
			left.z - right.z,
			left.w - right.w
			};
		}

		friend constexpr Vec4 operator *(const Vec4& left, const Vec4& right)
		{
			return {
			left.x * right.x,
			left.y * right.y,
			left.z * right.z,
			left.w * right.w
			};
		}

		friend constexpr Vec4 operator *(const Vec4& left, float scalar)
		{
			return {
			left.x * scalar,
			left.y * scalar,
			left.z * scalar,
			left.w * scalar
			};
		}

		friend constexpr Vec4 operator /(const Vec4& left, const Vec4& right)
		{
			return {
			left.x / right.x,
			left.y / right.y,
			left.z / right.z,
			left.w / right.w
			};
		}

		friend constexpr Vec4 operator /(const Vec4& left, float scalar)
		{
			return {
			left.x / scalar,
			left.y / scalar,
			left.z / scalar,
			left.w / scalar
			};
		}

		// assignment operators

		constexpr Vec4& operator  =(const Vec4& other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
			return *this;
		}

		constexpr Vec4& operator +=(const Vec4& other)
		{
			this->add(other);
			return *this;
		}
		
		constexpr Vec4& operator -=(const Vec4& other)
		{
			this->subtract(other);
			return *this;
		}

		constexpr Vec4& operator *=(const Vec4& other)
		{
			this->multiply(other);
			return *this;
		}

		constexpr Vec4& operator *=(float scalar)
		{
			this->multiply(scalar);
			return *this;
		}

		constexpr Vec4& operator /=(const Vec4& other)
		{
			this->divide(other);
			return *this;
		}

		constexpr Vec4& operator /=(float scalar)
		{
			this->divide(scalar);
			return *this;
		}

		// Unary Operators

		constexpr Vec4 operator +() const
		{
			return Vec4(+(this->x), +(this->y), +(this->z), +(this->w));
		}

		constexpr Vec4 operator -() const
		{
			return Vec4(-(this->x), -(this->y), -(this->z), -(this->w));
		}

		// Cast Operators

		constexpr operator float* () { return &x; }
		constexpr operator const float* () const { return &x; }

		// Extras / Helpers

		static inline float dot(const Vec4& left, const Vec4& right)
		{
			return (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w);
		}

	};

	constexpr Vec2::Vec2(const Vec4& other)
		: x(other.x), y(other.y)
	{
	}

	constexpr Vec3::Vec3(const Vec4& other)
		: x(other.x), y(other.y), z(other.z)
	{
	}

}

LOG_CREATE_FORMAT(daedalus::maths::Vec4, "x: {}, y: {}, z: {}, w: {}", vec, vec.x, vec.y, vec.z, vec.w);