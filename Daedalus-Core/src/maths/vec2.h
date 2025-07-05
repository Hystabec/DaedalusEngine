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

		Vec2& add(const Vec2& other);
		Vec2& subtract(const Vec2& other);
		Vec2& multiply(const Vec2& other);
		Vec2& divide(const Vec2& other);

		friend Vec2 operator +(Vec2 left, const Vec2& right);
		friend Vec2 operator -(Vec2 left, const Vec2& right);
		friend Vec2 operator *(Vec2 left, const Vec2& right);
		friend Vec2 operator /(Vec2 left, const Vec2& right);
		
		bool operator==(const Vec2& other);
		bool operator!=(const Vec2& other);

		Vec2& operator +=(const Vec2& other);
		Vec2& operator -=(const Vec2& other);
		Vec2& operator *=(const Vec2& other);
		Vec2& operator /=(const Vec2& other);

		operator float* () { return &x; }
	};

} }

LOG_CREATE_FORMAT(daedalus::maths::Vec2, "x: {}, y: {}", vec, vec.x, vec.y);