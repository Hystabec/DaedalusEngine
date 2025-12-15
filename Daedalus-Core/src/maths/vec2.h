#pragma once

namespace daedalus::maths {

	struct Vec3;
	struct Vec4;

	struct Vec2
	{
		float x, y;

		constexpr Vec2() noexcept
			: x(0.0f), y(0.0f)
		{
		}

		constexpr Vec2(float scalar) noexcept
			: x(scalar), y(scalar)
		{
		}

		constexpr Vec2(float x, float y) noexcept
			: x(x), y(y)
		{
		}

		constexpr Vec2(const Vec2& other) noexcept
			: x(other.x), y(other.y)
		{
		}

		constexpr Vec2(const Vec3& other) noexcept;
		constexpr Vec2(const Vec4& other) noexcept;

		// Locical Operators

		[[nodiscard]] constexpr bool operator ==(const Vec2& other) const noexcept
		{
			return x == other.x && y == other.y;
		}

		[[nodiscard]] constexpr bool operator !=(const Vec2& other) const noexcept
		{
			return !(x == other.x && y == other.y);
		}

		// Binary Arithmetic Operators
		
		/// @brief Modifies caller and returns result
		constexpr Vec2& add(const Vec2& other) noexcept
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec2& subtract(const Vec2& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec2& multiply(const Vec2& other) noexcept
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec2& multiply(float scalar) noexcept
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec2& divide(const Vec2& other) noexcept
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}
		/// @brief Modifies caller and returns result
		constexpr Vec2& divide(float scalar) noexcept
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		friend [[nodiscard]] constexpr Vec2 operator +(const Vec2& left, const Vec2& right) noexcept
		{
			return {
			left.x + right.x,
			left.y + right.y
			};
		}

		friend [[nodiscard]] constexpr Vec2 operator -(const Vec2& left, const Vec2& right) noexcept
		{
			return {
			left.x - right.x,
			left.y - right.y
			};
		}

		friend [[nodiscard]] constexpr Vec2 operator *(const Vec2& left, const Vec2& right) noexcept
		{
			return {
			left.x * right.x,
			left.y * right.y
			};
		}

		friend [[nodiscard]] constexpr Vec2 operator *(const Vec2& left, float scalar) noexcept
		{
			return {
			left.x * scalar,
			left.y * scalar
			};
		}

		friend [[nodiscard]] constexpr Vec2 operator /(const Vec2& left, const Vec2& right) noexcept
		{
			return {
			left.x / right.x,
			left.y / right.y
			};
		}

		friend [[nodiscard]] constexpr Vec2 operator /(const Vec2& left, float scalar) noexcept
		{
			return {
			left.x / scalar,
			left.y / scalar
			};
		}

		// assignment operators

		constexpr Vec2& operator  =(const Vec2& other) noexcept
		{
			this->x = other.x;
			this->y = other.y;
			return *this;
		}

		constexpr Vec2& operator +=(const Vec2& other) noexcept
		{
			this->add(other);
			return *this;
		}

		constexpr Vec2& operator -=(const Vec2& other) noexcept
		{
			this->subtract(other);
			return *this;
		}

		constexpr Vec2& operator *=(const Vec2& other) noexcept
		{
			this->multiply(other);
			return *this;
		}

		constexpr Vec2& operator *=(float scalar) noexcept
		{
			this->multiply(scalar);
			return *this;
		}

		constexpr Vec2& operator /=(const Vec2& other) noexcept
		{
			this->divide(other);
			return *this;
		}

		constexpr Vec2& operator /=(float scalar) noexcept
		{
			this->divide(scalar);
			return *this;
		}

		// Unary Operators

		[[nodiscard]] constexpr Vec2 operator +() const noexcept
		{
			return Vec2(+(this->x), +(this->y));
		}

		[[nodiscard]] constexpr Vec2 operator -() const noexcept
		{
			return Vec2(-(this->x), -(this->y));
		}

		// Cast Operators

		constexpr operator float* () noexcept { return &x; }
		constexpr operator const float* () const noexcept { return &x; }

		constexpr float& operator[](int index) noexcept(false)
		{
			if ((index < 0 || index > 1))
			{
				DD_CORE_ASSERT(false, "Index out of range");
				throw std::out_of_range("Index out of range");
			}
			return (&x)[index];
		}

		constexpr const float& operator[](int index) const noexcept(false)
		{
			if ((index < 0 || index > 1))
			{
				DD_CORE_ASSERT(false, "Index out of range");
				throw std::out_of_range("Index out of range");
			}
			return (&x)[index];
		}

		// Extras / Helpers

		static [[nodiscard]] constexpr float dot(const Vec2& left, const Vec2& right) noexcept
		{
			return (left.x * right.x) + (left.y * right.y);
		}

	};

}

LOG_CREATE_FORMAT(daedalus::maths::Vec2, "x: {}, y: {}", vec, vec.x, vec.y);