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

		constexpr Vec3() noexcept
			: x(0.0f), y(0.0f), z(0.0f)
		{
		}

		constexpr Vec3(float scalar) noexcept
			: x(scalar), y(scalar), z(scalar)
		{
		}

		constexpr Vec3(float x, float y, float z) noexcept
			: x(x), y(y), z(z)
		{
		}

		constexpr Vec3(const Vec3& other) noexcept
			: x(other.x), y(other.y), z(other.z)
		{
		}

		constexpr Vec3(const Vec2& other, float z = 0.0f) noexcept
			: x(other.x), y(other.y), z(z)
		{
		}

		constexpr Vec3(const Vec4& other) noexcept;

		// Locical Operators

		[[nodiscard]] constexpr bool operator ==(const Vec3& other) const noexcept
		{
			return (x == other.x && y == other.y && z == other.z);
		}

		[[nodiscard]] constexpr bool operator !=(const Vec3& other) const noexcept
		{
			return !(x == other.x && y == other.y && z == other.z);
		}

		// Binary Arithmetic Operators

		/// @brief Modifies caller and returns result
		constexpr Vec3& add(const Vec3& other) noexcept
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec3& subtract(const Vec3& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec3& multiply(const Vec3& other) noexcept
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec3& multiply(float scalar) noexcept
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec3& divide(const Vec3& other) noexcept
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec3& divide(float scalar) noexcept
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		friend [[nodiscard]] constexpr Vec3 operator +(const Vec3& left, const Vec3& right) noexcept
		{
			return {
			left.x + right.x,
			left.y + right.y,
			left.z + right.z
			};
		}

		friend [[nodiscard]] constexpr Vec3 operator -(const Vec3& left, const Vec3& right) noexcept
		{
			return {
			left.x - right.x,
			left.y - right.y,
			left.z - right.z
			};
		}

		friend [[nodiscard]] constexpr Vec3 operator *(const Vec3& left, const Vec3& right) noexcept
		{
			return {
			left.x * right.x,
			left.y * right.y,
			left.z * right.z
			};
		}

		friend [[nodiscard]] constexpr Vec3 operator *(const Vec3& left, float scalar) noexcept
		{
			return {
			left.x * scalar,
			left.y * scalar,
			left.z * scalar
			};
		}

		friend [[nodiscard]] constexpr Vec3 operator /(const Vec3& left, const Vec3& right) noexcept
		{
			return {
			left.x / right.x,
			left.y / right.y,
			left.z / right.z
			};
		}

		friend [[nodiscard]] constexpr Vec3 operator /(const Vec3& left, float scalar) noexcept
		{
			return {
			left.x / scalar,
			left.y / scalar,
			left.z / scalar
			};
		}

		// assignment operators

		constexpr Vec3& operator  =(const Vec3& other) noexcept
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;
		}

		constexpr Vec3& operator +=(const Vec3& other) noexcept
		{
			this->add(other);
			return *this;
		}

		constexpr Vec3& operator -=(const Vec3& other) noexcept
		{

			this->subtract(other);
			return *this;
		}

		constexpr Vec3& operator *=(const Vec3& other) noexcept
		{
			this->multiply(other);
			return *this;
		}

		constexpr Vec3& operator *=(float scalar) noexcept
		{
			this->multiply(scalar);
			return *this;
		}

		constexpr Vec3& operator /=(const Vec3& other) noexcept
		{
			this->divide(other);
			return *this;
		}

		constexpr Vec3& operator /=(float scalar) noexcept
		{
			this->divide(scalar);
			return *this;
		}

		// Unary Operators

		[[nodiscard]] constexpr Vec3 operator +() const noexcept
		{
			return Vec3(+(this->x), +(this->y), +(this->z));
		}

		[[nodiscard]] constexpr Vec3 operator -() const noexcept
		{
			return Vec3(-(this->x), -(this->y), -(this->z));
		}

		// Cast Operators

		constexpr operator float* () noexcept { return &x; }
		constexpr operator const float* () const noexcept { return &x; }

		constexpr float& operator[](int index) noexcept(false)
		{
			if ((index < 0 || index > 2))
			{
				DD_CORE_ASSERT(false, "Index out of range");
				throw std::out_of_range("Index out of range");
			}
			return (&x)[index];
		}

		constexpr const float& operator[](int index) const noexcept(false)
		{
			if ((index < 0 || index > 2))
			{
				DD_CORE_ASSERT(false, "Index out of range");
				throw std::out_of_range("Index out of range");
			}
			return (&x)[index];
		}

		// Extras / Helpers

		static [[nodiscard]] constexpr float dot(const Vec3& left, const Vec3& right) noexcept
		{
			return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
		}

		static [[nodiscard]] constexpr Vec3 cross(const Vec3& left, const Vec3& right) noexcept
		{
			return {
			left.y * right.z - right.y * left.z,
			left.z * right.x - right.z * left.x,
			left.x * right.y - right.x * left.y
			};
		}

		static [[nodiscard]] constexpr float length(const Vec3& vec3) noexcept
		{
			return sqrt(Vec3::dot(vec3, vec3));
		}

		static [[nodiscard]] constexpr Vec3 scale(const Vec3& vec3, float desiredLength) noexcept
		{
			return vec3 * desiredLength / length(vec3);
		}

	};

	constexpr Vec2::Vec2(const Vec3& other) noexcept
		: x(other.x), y(other.y)
	{
	}

}

LOG_CREATE_FORMAT(daedalus::maths::Vec3, "x: {}, y: {}, z: {}", vec, vec.x, vec.y, vec.z);