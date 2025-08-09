#pragma once

namespace daedalus::maths {

	struct Vec3;
	struct Vec4;

	struct Vec2
	{
		float x, y;

		constexpr Vec2()
			: x(0.0f), y(0.0f)
		{
		}

		constexpr Vec2(float scalar)
			: x(scalar), y(scalar)
		{
		}

		constexpr Vec2(float x, float y)
			: x(x), y(y)
		{
		}

		constexpr Vec2(const Vec2& other)
			: x(other.x), y(other.y)
		{
		}

		constexpr Vec2(const Vec3& other);
		constexpr Vec2(const Vec4& other);

		// Locical Operators

		constexpr bool operator ==(const Vec2& other) const
		{
			return x == other.x && y == other.y;
		}

		constexpr bool operator !=(const Vec2& other) const
		{
			return !(x == other.x && y == other.y);
		}

		// Binary Arithmetic Operators
		
		/// @brief Modifies caller and returns result
		constexpr Vec2& add(const Vec2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec2& subtract(const Vec2& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec2& multiply(const Vec2& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec2& multiply(float scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec2& divide(const Vec2& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec2& divide(float scalar)
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		friend constexpr Vec2 operator +(const Vec2& left, const Vec2& right)
		{
			return {
			left.x + right.x,
			left.y + right.y
			};
		}

		friend constexpr Vec2 operator -(const Vec2& left, const Vec2& right)
		{
			return {
			left.x - right.x,
			left.y - right.y
			};
		}

		friend constexpr Vec2 operator *(const Vec2& left, const Vec2& right)
		{
			return {
			left.x * right.x,
			left.y * right.y
			};
		}

		friend constexpr Vec2 operator *(const Vec2& left, float scalar)
		{
			return {
			left.x * scalar,
			left.y * scalar
			};
		}

		friend constexpr Vec2 operator /(const Vec2& left, const Vec2& right)
		{
			return {
			left.x / right.x,
			left.y / right.y
			};
		}

		friend constexpr Vec2 operator /(const Vec2& left, float scalar)
		{
			return {
			left.x / scalar,
			left.y / scalar
			};
		}

		// assignment operators

		constexpr Vec2& operator  =(const Vec2& other)
		{
			this->x = other.x;
			this->y = other.y;
			return *this;
		}

		constexpr Vec2& operator +=(const Vec2& other)
		{
			this->add(other);
			return *this;
		}

		constexpr Vec2& operator -=(const Vec2& other)
		{
			this->subtract(other);
			return *this;
		}

		constexpr Vec2& operator *=(const Vec2& other)
		{
			this->multiply(other);
			return *this;
		}

		constexpr Vec2& operator *=(float scalar)
		{
			this->multiply(scalar);
			return *this;
		}

		constexpr Vec2& operator /=(const Vec2& other)
		{
			this->divide(other);
			return *this;
		}

		constexpr Vec2& operator /=(float scalar)
		{
			this->divide(scalar);
			return *this;
		}

		// Unary Operators

		constexpr Vec2 operator +() const
		{
			return Vec2(+(this->x), +(this->y));
		}

		constexpr Vec2 operator -() const
		{
			return Vec2(-(this->x), -(this->y));
		}

		// Cast Operators

		constexpr operator float* () { return &x; }
		constexpr operator const float* () const { return &x; }

		// Extras / Helpers

		static inline float dot(const Vec2& left, const Vec2& right)
		{
			return (left.x * right.x) + (left.y * right.y);
		}

	};

}

LOG_CREATE_FORMAT(daedalus::maths::Vec2, "x: {}, y: {}", vec, vec.x, vec.y);