#pragma once

namespace daedalus { namespace maths {

	struct Vec3
	{
		float x, y, z;

		Vec3();
		Vec3(const float& x);
		Vec3(const float& x, const float& y, const float& z);
		Vec3(const Vec3& other);

		Vec3& add(const Vec3& other);
		Vec3& subtract(const Vec3& other);
		Vec3& multiply(const Vec3& other);
		Vec3& divide(const Vec3& other);

		friend Vec3 operator +(Vec3 left, const Vec3& right);
		friend Vec3 operator -(Vec3 left, const Vec3& right);
		friend Vec3 operator *(Vec3 left, const Vec3& right);
		friend Vec3 operator /(Vec3 left, const Vec3& right);
		
		bool operator==(const Vec3& other);
		bool operator!=(const Vec3& other);

		Vec3& operator +=(const Vec3& other);
		Vec3& operator -=(const Vec3& other);
		Vec3& operator *=(const Vec3& other);
		Vec3& operator /=(const Vec3& other);

		operator float* () { return &x; }
	};

} }

LOG_CREATE_FORMAT(daedalus::maths::Vec3, "x: {}, y: {}, z: {}", vec, vec.x, vec.y, vec.z);