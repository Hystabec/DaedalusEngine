#pragma once

#include "vec3.h"
#include "vec4.h"

namespace daedalus { namespace maths {

	struct Mat4
	{
		//column-major

		union 
		{
			float elements[16];
			Vec4 columns[4];
		};

		Mat4();
		Mat4(float diagonal);
		Mat4(float* elements);
		Mat4(const Vec4& column0, const Vec4& column1, const Vec4& column2, const Vec4& column3);
		Mat4(const Mat4& other);

		// Locical Operators

		bool operator ==(const Mat4& other);
		bool operator !=(const Mat4& other);

		// Binary Arithmetic Operators

		/// @brief Modifies caller and returns result
		Mat4& multiply(const Mat4& other);
		/// @brief Modifies caller and returns result
		Mat4& multiply(float other);

		friend Mat4 operator *(const Mat4& left, const Mat4& right);
		friend Mat4 operator *(const Mat4& left, float right);
		friend Vec3 operator *(const Mat4& left, const Vec3& right);
		friend Vec4 operator *(const Mat4& left, const Vec4& right);

		Mat4& operator *=(const Mat4& other);
		Mat4& operator *=(float other);

		// Cast Operators

		operator float* () { return elements; }
		operator const float* () const { return elements; }

		// Extras / Helpers

		static Mat4 identity();

		Mat4& invert();
		static Mat4 invert(const Mat4& matrix);

		static Mat4 orthographic(float left, float right, float botton, float top, float nearPlane, float farPlane);
		/// @param fov - in radians
		static Mat4 perspective(float fov, float aspectRatio, float nearPlane, float farPlane);

		static Mat4 translate(const Vec3& translation);
		/// @brief Returns an indenty matrix rotated by the specified angle and axis. 
		/// A bool can be passed in as the final peramiter to pass angle as degrees.
		static Mat4 rotate(float angle, const Vec3& axis, bool inRadians = true);
		static Mat4 scale(const Vec3& scale);

		static bool decomposeTransform(const Mat4& transform, Vec3& outPosition, Vec3& outRotation, Vec3& outScale);
	};

} }

LOG_CREATE_FORMAT(daedalus::maths::Mat4, "{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}", 
	thisMat4, 
	thisMat4.elements[0], thisMat4.elements[1], thisMat4.elements[2], thisMat4.elements[3], 
	thisMat4.elements[4], thisMat4.elements[5], thisMat4.elements[6], thisMat4.elements[7],
	thisMat4.elements[8], thisMat4.elements[9], thisMat4.elements[10], thisMat4.elements[11],
	thisMat4.elements[12], thisMat4.elements[13], thisMat4.elements[14], thisMat4.elements[15])