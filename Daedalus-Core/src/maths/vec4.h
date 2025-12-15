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


		constexpr Vec4() noexcept
			: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{
		}

		constexpr Vec4(float scalar) noexcept
			: x(scalar), y(scalar), z(scalar), w(scalar)
		{
		}

		constexpr Vec4(float x, float y, float z, float w) noexcept
			: x(x), y(y), z(z), w(w)
		{
		}

		constexpr Vec4(const Vec4& other) noexcept
			: x(other.x), y(other.y), z(other.z), w(other.w)
		{
		}

		constexpr Vec4(const Vec2& other, float z = 0.0f, float w = 0.0f) noexcept
			: x(other.x), y(other.y), z(z), w(w)
		{
		}

		constexpr Vec4(const Vec3& other, float w = 0.0f) noexcept
			: x(other.x), y(other.y), z(other.z), w(w)
		{
		}

		// Locical Operators

		[[nodiscard]] constexpr bool operator ==(const Vec4& other) const noexcept
		{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}

		[[nodiscard]] constexpr bool operator !=(const Vec4& other) const noexcept
		{
			return !(x == other.x && y == other.y && z == other.z && w == other.w);
		}

		// Binary Arithmetic Operators

		/// @brief Modifies caller and returns result
		constexpr Vec4& add(const Vec4& other) noexcept
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec4& subtract(const Vec4& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec4& multiply(const Vec4& other) noexcept
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec4& multiply(float scalar) noexcept
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec4& divide(const Vec4& other) noexcept
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec4& divide(float scalar) noexcept
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
			return *this;
		}

		friend [[nodiscard]] constexpr Vec4 operator +(const Vec4& left, const Vec4& right) noexcept
		{
			return {
			left.x + right.x,
			left.y + right.y,
			left.z + right.z,
			left.w + right.w
			};
		}

		friend [[nodiscard]] constexpr Vec4 operator -(const Vec4& left, const Vec4& right) noexcept
		{
			return {
			left.x - right.x,
			left.y - right.y,
			left.z - right.z,
			left.w - right.w
			};
		}

		friend [[nodiscard]] constexpr Vec4 operator *(const Vec4& left, const Vec4& right) noexcept
		{
			return {
			left.x * right.x,
			left.y * right.y,
			left.z * right.z,
			left.w * right.w
			};
		}

		friend [[nodiscard]] constexpr Vec4 operator *(const Vec4& left, float scalar) noexcept
		{
			return {
			left.x * scalar,
			left.y * scalar,
			left.z * scalar,
			left.w * scalar
			};
		}

		friend [[nodiscard]] constexpr Vec4 operator /(const Vec4& left, const Vec4& right) noexcept
		{
			return {
			left.x / right.x,
			left.y / right.y,
			left.z / right.z,
			left.w / right.w
			};
		}

		friend [[nodiscard]] constexpr Vec4 operator /(const Vec4& left, float scalar) noexcept
		{
			return {
			left.x / scalar,
			left.y / scalar,
			left.z / scalar,
			left.w / scalar
			};
		}

		// assignment operators

		constexpr Vec4& operator  =(const Vec4& other) noexcept
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
			return *this;
		}

		constexpr Vec4& operator +=(const Vec4& other) noexcept
		{
			this->add(other);
			return *this;
		}
		
		constexpr Vec4& operator -=(const Vec4& other) noexcept
		{
			this->subtract(other);
			return *this;
		}

		constexpr Vec4& operator *=(const Vec4& other) noexcept
		{
			this->multiply(other);
			return *this;
		}

		constexpr Vec4& operator *=(float scalar) noexcept
		{
			this->multiply(scalar);
			return *this;
		}

		constexpr Vec4& operator /=(const Vec4& other) noexcept
		{
			this->divide(other);
			return *this;
		}

		constexpr Vec4& operator /=(float scalar) noexcept
		{
			this->divide(scalar);
			return *this;
		}

		// Unary Operators

		[[nodiscard]] constexpr Vec4 operator +() const noexcept
		{
			return Vec4(+(this->x), +(this->y), +(this->z), +(this->w));
		}

		[[nodiscard]]  constexpr Vec4 operator -() const noexcept
		{
			return Vec4(-(this->x), -(this->y), -(this->z), -(this->w));
		}

		// Cast Operators

		constexpr operator float* () noexcept { return &x; }
		constexpr operator const float* () const noexcept { return &x; }

		constexpr float& operator[](int index) noexcept(false)
		{
			if ((index < 0 || index > 3))
			{
				DD_CORE_ASSERT(false, "Index out of range");
				throw std::out_of_range("Index out of range");
			}
			return (&x)[index];
		}

		constexpr const float& operator[](int index) const noexcept(false)
		{
			if ((index < 0 || index > 3))
			{
				DD_CORE_ASSERT(false, "Index out of range");
				throw std::out_of_range("Index out of range");
			}
			return (&x)[index];
		}

		// Extras / Helpers

		static [[nodiscard]] constexpr float dot(const Vec4& left, const Vec4& right) noexcept
		{
			return (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w);
		}

	};

	constexpr Vec2::Vec2(const Vec4& other) noexcept
		: x(other.x), y(other.y)
	{
	}

	constexpr Vec3::Vec3(const Vec4& other) noexcept
		: x(other.x), y(other.y), z(other.z)
	{
	}

}

LOG_CREATE_FORMAT(daedalus::maths::Vec4, "x: {}, y: {}, z: {}, w: {}", vec, vec.x, vec.y, vec.z, vec.w);