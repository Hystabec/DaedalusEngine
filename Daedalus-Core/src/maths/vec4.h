#pragma once

namespace daedalus { namespace maths {

	struct Vec2;
	struct Vec3;

	struct Vec4
	{
		float x, y, z, w;

		Vec4();
		Vec4(const float& x);
		Vec4(const float& x, const float& y, const float& z, const float& w);
		Vec4(const Vec4& other);

		Vec4(const Vec2& other, float z = 0.0f, float w = 0.0f);
		Vec4(const Vec3& other, float w = 0.0f);

		// Locical Operators

		bool operator ==(const Vec4& other);
		bool operator !=(const Vec4& other);

		// Binary Arithmetic Operators

		/// @brief Modifies caller and returns result
		Vec4& add(const Vec4& other);
		/// @brief Modifies caller and returns result
		Vec4& subtract(const Vec4& other);
		/// @brief Modifies caller and returns result
		Vec4& multiply(const Vec4& other);
		/// @brief Modifies caller and returns result
		Vec4& multiply(float scalar);
		/// @brief Modifies caller and returns result
		Vec4& divide(const Vec4& other);
		/// @brief Modifies caller and returns result
		Vec4& divide(float scalar);

		friend Vec4 operator +(const Vec4& left, const Vec4& right);
		friend Vec4 operator -(const Vec4& left, const Vec4& right);
		friend Vec4 operator *(const Vec4& left, const Vec4& right);
		friend Vec4 operator *(const Vec4& left, float scalar);
		friend Vec4 operator /(const Vec4& left, const Vec4& right);
		friend Vec4 operator /(const Vec4& left, float scalar);

		// assignment operators

		Vec4& operator  =(const Vec4& other);
		Vec4& operator +=(const Vec4& other);
		Vec4& operator -=(const Vec4& other);
		Vec4& operator *=(const Vec4& other);
		Vec4& operator *=(float scalar);
		Vec4& operator /=(const Vec4& other);
		Vec4& operator /=(float scalar);

		// Unary Operators

		Vec4 operator +() const;
		Vec4 operator -() const;

		// Cast Operators

		operator float* () { return &x; }
		operator const float* () const { return &x; }

		// Extras / Helpers

		static float dot(const Vec4& left, const Vec4& right);
	};

} }

LOG_CREATE_FORMAT(daedalus::maths::Vec4, "x: {}, y: {}, z: {}, w: {}", vec, vec.x, vec.y, vec.z, vec.w);