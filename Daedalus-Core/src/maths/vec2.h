#pragma once
#include "vec4.h"

namespace daedalus { namespace maths {

	struct Vec3;
	struct Vec4;

	struct Vec2
	{
		float x, y;

		Vec2();
		Vec2(const float& x);
		Vec2(const float& x, const float& y);
		Vec2(const Vec2& other);

		Vec2(const Vec3& other);
		Vec2(const Vec4& other);

		// Locical Operators

		bool operator ==(const Vec2& other);
		bool operator !=(const Vec2& other);

		// Binary Arithmetic Operators
		
		/// @brief Modifies caller and returns result
		Vec2& add(const Vec2& other);
		/// @brief Modifies caller and returns result
		Vec2& subtract(const Vec2& other);
		/// @brief Modifies caller and returns result
		Vec2& multiply(const Vec2& other);
		/// @brief Modifies caller and returns result
		Vec2& multiply(float scalar);
		/// @brief Modifies caller and returns result
		Vec2& divide(const Vec2& other);
		/// @brief Modifies caller and returns result
		Vec2& divide(float scalar);

		friend Vec2 operator +(const Vec2& left, const Vec2& right);
		friend Vec2 operator -(const Vec2& left, const Vec2& right);
		friend Vec2 operator *(const Vec2& left, const Vec2& right);
		friend Vec2 operator *(const Vec2& left, float scalar);
		friend Vec2 operator /(const Vec2& left, const Vec2& right);
		friend Vec2 operator /(const Vec2& left, float scalar);

		// assignment operators

		Vec2& operator  =(const Vec2& other);
		Vec2& operator +=(const Vec2& other);
		Vec2& operator -=(const Vec2& other);
		Vec2& operator *=(const Vec2& other);
		Vec2& operator *=(float scalar);
		Vec2& operator /=(const Vec2& other);
		Vec2& operator /=(float scalar);

		// Unary Operators

		Vec2 operator +() const;
		Vec2 operator -() const;

		// Cast Operators

		operator float* () { return &x; }
		operator const float* () const { return &x; }

		// Extras / Helpers

		static float dot(const Vec2& left, const Vec2& right);
	};

} }

LOG_CREATE_FORMAT(daedalus::maths::Vec2, "x: {}, y: {}", vec, vec.x, vec.y);