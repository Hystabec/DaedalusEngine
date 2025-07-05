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

		Vec4& add(const Vec4& other);
		Vec4& subtract(const Vec4& other);
		Vec4& multiply(const Vec4& other);
		Vec4& divide(const Vec4& other);

		friend Vec4 operator +(Vec4 left, const Vec4& right);
		friend Vec4 operator -(Vec4 left, const Vec4& right);
		friend Vec4 operator *(Vec4 left, const Vec4& right);
		friend Vec4 operator /(Vec4 left, const Vec4& right);
		
		bool operator==(const Vec4& other);
		bool operator!=(const Vec4& other);

		Vec4& operator +=(const Vec4& other);
		Vec4& operator -=(const Vec4& other);
		Vec4& operator *=(const Vec4& other);
		Vec4& operator /=(const Vec4& other);

		operator float* () { return &x; }
	};

} }

LOG_CREATE_FORMAT(daedalus::maths::Vec4, "x: {}, y: {}, z: {}, w: {}", vec, vec.x, vec.y, vec.z, vec.w);