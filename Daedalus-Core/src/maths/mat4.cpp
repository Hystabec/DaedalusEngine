#include "ddpch.h"
#include "mat4.h"

#include "maths.h"
#include <limits>

namespace daedalus { namespace maths {

		Mat4::Mat4()
		{
			memset(elements, 0, sizeof(float) * 16);
		}

		Mat4::Mat4(float diaganol)
		{
			memset(elements, 0, sizeof(float) * 16);

			elements[0 + 0 * 4] = diaganol;
			elements[1 + 1 * 4] = diaganol;
			elements[2 + 2 * 4] = diaganol;
			elements[3 + 3 * 4] = diaganol;
		}

		Mat4::Mat4(float* elements)
		{
			memcpy(this->elements, elements, sizeof(float) * 16);
		}

		Mat4::Mat4(const Vec4& column0, const Vec4& column1, const Vec4& column2, const Vec4& column3)
		{
			columns[0] = column0;
			columns[1] = column1;
			columns[2] = column2;
			columns[3] = column3;
		}

		Mat4::Mat4(const Mat4& other)
		{
			memcpy(this->elements, other.elements, sizeof(float) * 16);
		}

		bool Mat4::operator==(const Mat4& other)
		{
			for (int i = 0; i < 4 * 4; i++)
				if (this->elements[i] != other.elements[i])
					return false;

			return true;
		}

		bool Mat4::operator!=(const Mat4& other)
		{
			for (int i = 0; i < 4 * 4; i++)
				if (this->elements[i] != other.elements[i])
					return true;

			return false;
		}

		Mat4& Mat4::multiply(const Mat4& other)
		{
			float data[16];
			for (int col = 0; col < 4; col++)
			{
				for (int row = 0; row < 4; row++)
				{
					float sum = 0.0f;
					for (int elm = 0; elm < 4; elm++)
					{
						sum += elements[row + elm * 4] * other.elements[elm + col * 4];
					}
					data[row + col * 4] = sum;
				}
			}

			memcpy(elements, data, 4 * 4 * sizeof(float));

			return *this;
		}

		Mat4& Mat4::multiply(float other)
		{
			float data[16];
			for (int col = 0; col < 4; col++)
			{
				for (int row = 0; row < 4; row++)
				{
					float sum = 0.0f;
					for (int elm = 0; elm < 4; elm++)
					{
						sum += elements[row + elm * 4] * other;
					}
					data[row + col * 4] = sum;
				}
			}

			memcpy(elements, data, 4 * 4 * sizeof(float));

			return *this;
		}

		Mat4 operator*(const Mat4& left, const Mat4& right)
		{
			Mat4 result(left);
			result.multiply(right);
			return result;
		}

		Mat4 operator*(const Mat4& left, float right)
		{
			return {
				left.columns[0] * right,
				left.columns[1] * right,
				left.columns[2] * right,
				left.columns[3] * right
			};
		}

		Vec3 operator*(const Mat4& left, const Vec3& right)
		{
			return Vec3
			(
				left.elements[0 + 0 * 4] * right.x + left.elements[0 + 1 * 4] * right.y + left.elements[0 + 2 * 4] * right.z + left.elements[0 + 3 * 4],
				left.elements[1 + 0 * 4] * right.x + left.elements[1 + 1 * 4] * right.y + left.elements[1 + 2 * 4] * right.z + left.elements[1 + 3 * 4],
				left.elements[2 + 0 * 4] * right.x + left.elements[2 + 1 * 4] * right.y + left.elements[2 + 2 * 4] * right.z + left.elements[2 + 3 * 4]
			);
		}

		Vec4 operator*(const Mat4& left, const Vec4& right)
		{
			return Vec4
			(
				left.elements[0 + 0 * 4] * right.x + left.elements[0 + 1 * 4] * right.y + left.elements[0 + 2 * 4] * right.z + left.elements[0 + 3 * 4] * right.w,
				left.elements[1 + 0 * 4] * right.x + left.elements[1 + 1 * 4] * right.y + left.elements[1 + 2 * 4] * right.z + left.elements[1 + 3 * 4] * right.w,
				left.elements[2 + 0 * 4] * right.x + left.elements[2 + 1 * 4] * right.y + left.elements[2 + 2 * 4] * right.z + left.elements[2 + 3 * 4] * right.w,
				left.elements[3 + 0 * 4] * right.x + left.elements[3 + 1 * 4] * right.y + left.elements[3 + 2 * 4] * right.z + left.elements[3 + 3 * 4] * right.w
			);
		}

		Mat4& Mat4::operator*=(const Mat4& other)
		{
			return this->multiply(other);
		}

		Mat4& Mat4::operator*=(float other)
		{
			return this->multiply(other);
		}

		Mat4 Mat4::identity()
		{
			return Mat4(1.0f);
		}

		Mat4& Mat4::invert()
		{
			// Copied from GLM

			float coef00 = this->columns[2][2] * this->columns[3][3] - this->columns[3][2] * this->columns[2][3];
			float coef02 = this->columns[1][2] * this->columns[3][3] - this->columns[3][2] * this->columns[1][3];
			float coef03 = this->columns[1][2] * this->columns[2][3] - this->columns[2][2] * this->columns[1][3];

			float coef04 = this->columns[2][1] * this->columns[3][3] - this->columns[3][1] * this->columns[2][3];
			float coef06 = this->columns[1][1] * this->columns[3][3] - this->columns[3][1] * this->columns[1][3];
			float coef07 = this->columns[1][1] * this->columns[2][3] - this->columns[2][1] * this->columns[1][3];

			// values go slight off in this block 
			float coef08 = this->columns[2][1] * this->columns[3][2] - this->columns[3][1] * this->columns[2][2];
			float coef10 = this->columns[1][1] * this->columns[3][2] - this->columns[3][1] * this->columns[1][2];
			float coef11 = this->columns[1][1] * this->columns[2][2] - this->columns[2][1] * this->columns[1][2];

			float coef12 = this->columns[2][0] * this->columns[3][3] - this->columns[3][0] * this->columns[2][3];
			float coef14 = this->columns[1][0] * this->columns[3][3] - this->columns[3][0] * this->columns[1][3];
			float coef15 = this->columns[1][0] * this->columns[2][3] - this->columns[2][0] * this->columns[1][3];

			// values here go completly off in this block
			float coef16 = this->columns[2][0] * this->columns[3][2] - this->columns[3][0] * this->columns[2][2];
			float coef18 = this->columns[1][0] * this->columns[3][2] - this->columns[3][0] * this->columns[1][2];
			float coef19 = this->columns[1][0] * this->columns[2][2] - this->columns[2][0] * this->columns[1][2];

			float coef20 = this->columns[2][0] * this->columns[3][1] - this->columns[3][0] * this->columns[2][1]; // this is very wrong
			float coef22 = this->columns[1][0] * this->columns[3][1] - this->columns[3][0] * this->columns[1][1]; // this is slight wrong
			float coef23 = this->columns[1][0] * this->columns[2][1] - this->columns[2][0] * this->columns[1][1];

			Vec4 fac0(coef00, coef00, coef02, coef03);
			Vec4 fac1(coef04, coef04, coef06, coef07);
			Vec4 fac2(coef08, coef08, coef10, coef11);
			Vec4 fac3(coef12, coef12, coef14, coef15);
			Vec4 fac4(coef16, coef16, coef18, coef19);
			Vec4 fac5(coef20, coef20, coef22, coef23);

			Vec4 vec0(this->columns[1][0], this->columns[0][0], this->columns[0][0], this->columns[0][0]);
			Vec4 vec1(this->columns[1][1], this->columns[0][1], this->columns[0][1], this->columns[0][1]);
			Vec4 vec2(this->columns[1][2], this->columns[0][2], this->columns[0][2], this->columns[0][2]);
			Vec4 vec3(this->columns[1][3], this->columns[0][3], this->columns[0][3], this->columns[0][3]);

			Vec4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
			Vec4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
			Vec4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
			Vec4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

			Vec4 signA(+1, -1, +1, -1);
			Vec4 signB(-1, +1, -1, +1);
			Mat4 inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

			Vec4 row0(inverse.columns[0][0], inverse.columns[1][0], inverse.columns[2][0], inverse.columns[3][0]);

			Vec4 dot0(this->columns[0] * row0);
			float dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

			float oneOverDeterminant = 1.0f / dot1;

			memcpy(elements, (inverse * oneOverDeterminant).elements, 4 * 4 * sizeof(float));
			return *this;
		}

		Mat4 Mat4::invert(const Mat4& matrix)
		{
			// Copied from GLM

			float coef00 = matrix.columns[2][2] * matrix.columns[3][3] - matrix.columns[3][2] * matrix.columns[2][3];
			float coef02 = matrix.columns[1][2] * matrix.columns[3][3] - matrix.columns[3][2] * matrix.columns[1][3];
			float coef03 = matrix.columns[1][2] * matrix.columns[2][3] - matrix.columns[2][2] * matrix.columns[1][3];

			float coef04 = matrix.columns[2][1] * matrix.columns[3][3] - matrix.columns[3][1] * matrix.columns[2][3];
			float coef06 = matrix.columns[1][1] * matrix.columns[3][3] - matrix.columns[3][1] * matrix.columns[1][3];
			float coef07 = matrix.columns[1][1] * matrix.columns[2][3] - matrix.columns[2][1] * matrix.columns[1][3];

			// values go slight off in this block 
			float coef08 = matrix.columns[2][1] * matrix.columns[3][2] - matrix.columns[3][1] * matrix.columns[2][2];
			float coef10 = matrix.columns[1][1] * matrix.columns[3][2] - matrix.columns[3][1] * matrix.columns[1][2];
			float coef11 = matrix.columns[1][1] * matrix.columns[2][2] - matrix.columns[2][1] * matrix.columns[1][2];

			float coef12 = matrix.columns[2][0] * matrix.columns[3][3] - matrix.columns[3][0] * matrix.columns[2][3];
			float coef14 = matrix.columns[1][0] * matrix.columns[3][3] - matrix.columns[3][0] * matrix.columns[1][3];
			float coef15 = matrix.columns[1][0] * matrix.columns[2][3] - matrix.columns[2][0] * matrix.columns[1][3];

			// values here go completly off in this block
			float coef16 = matrix.columns[2][0] * matrix.columns[3][2] - matrix.columns[3][0] * matrix.columns[2][2];
			float coef18 = matrix.columns[1][0] * matrix.columns[3][2] - matrix.columns[3][0] * matrix.columns[1][2];
			float coef19 = matrix.columns[1][0] * matrix.columns[2][2] - matrix.columns[2][0] * matrix.columns[1][2];

			float coef20 = matrix.columns[2][0] * matrix.columns[3][1] - matrix.columns[3][0] * matrix.columns[2][1]; // this is very wrong
			float coef22 = matrix.columns[1][0] * matrix.columns[3][1] - matrix.columns[3][0] * matrix.columns[1][1]; // this is slight wrong
			float coef23 = matrix.columns[1][0] * matrix.columns[2][1] - matrix.columns[2][0] * matrix.columns[1][1];

			Vec4 fac0(coef00, coef00, coef02, coef03);
			Vec4 fac1(coef04, coef04, coef06, coef07);
			Vec4 fac2(coef08, coef08, coef10, coef11);
			Vec4 fac3(coef12, coef12, coef14, coef15);
			Vec4 fac4(coef16, coef16, coef18, coef19);
			Vec4 fac5(coef20, coef20, coef22, coef23);

			Vec4 vec0(matrix.columns[1][0], matrix.columns[0][0], matrix.columns[0][0], matrix.columns[0][0]);
			Vec4 vec1(matrix.columns[1][1], matrix.columns[0][1], matrix.columns[0][1], matrix.columns[0][1]);
			Vec4 vec2(matrix.columns[1][2], matrix.columns[0][2], matrix.columns[0][2], matrix.columns[0][2]);
			Vec4 vec3(matrix.columns[1][3], matrix.columns[0][3], matrix.columns[0][3], matrix.columns[0][3]);

			Vec4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
			Vec4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
			Vec4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
			Vec4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

			Vec4 signA(+1, -1, +1, -1);
			Vec4 signB(-1, +1, -1, +1);
			Mat4 inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

			Vec4 row0(inverse.columns[0][0], inverse.columns[1][0], inverse.columns[2][0], inverse.columns[3][0]);

			Vec4 dot0(matrix.columns[0] * row0);
			float dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

			float oneOverDeterminant = 1.0f / dot1;

			return inverse * oneOverDeterminant;
		}

		Mat4 Mat4::orthographic(float left, float right, float botton, float top, float nearPlane, float farPlane)
		{
			Mat4 result(1.0f);

			result.elements[0 + 0 * 4] = 2.0f / (right - left);
			result.elements[1 + 1 * 4] = 2.0f / (top - botton);
			result.elements[2 + 2 * 4] = 2.0f / (nearPlane - farPlane);

			result.elements[0 + 3 * 4] = (left + right) / (left - right);
			result.elements[1 + 3 * 4] = (botton + top) / (botton - top);
			result.elements[2 + 3 * 4] = (farPlane + nearPlane) / (farPlane - nearPlane);

			return result;
		}

		Mat4 Mat4::perspective(float fov, float aspectRatio, float nearPlane, float farPlane)
		{
			Mat4 result(1.0f);
			
			float q = 1.0f / tan(fov / 2);
			float a = q / aspectRatio;
			
			result.elements[0 + 0 * 4] = a;
			result.elements[1 + 1 * 4] = q;
			result.elements[2 + 2 * 4] = ((nearPlane + farPlane) / (nearPlane - farPlane));
			result.elements[3 + 2 * 4] = -1.0f;
			result.elements[2 + 3 * 4] = ((2.0f * nearPlane * farPlane) / (nearPlane - farPlane));

			return result;
		}

		Mat4 Mat4::translate(const Vec3& translation)
		{
			Mat4 result(1.0f);

			result.elements[0 + 3 * 4] = translation.x;
			result.elements[1 + 3 * 4] = translation.y;
			result.elements[2 + 3 * 4] = translation.z;

			return result;
		}

		Mat4 Mat4::rotate(float angle, const Vec3& axis, bool inRadians)
		{
			Mat4 result(1.0f);

			float asRads = inRadians ? angle : degrees_to_radians(angle);
			float c = cos(asRads);
			float s = sin(asRads);
			float omc = 1.0f - c;

			result.elements[0 + 0 * 4] = axis.x * axis.x * omc + c;
			result.elements[1 + 0 * 4] = axis.y * axis.x * omc + axis.z * s;
			result.elements[2 + 0 * 4] = axis.x * axis.z * omc - axis.y * s;

			result.elements[0 + 1 * 4] = axis.x * axis.y * omc - axis.z * s;
			result.elements[1 + 1 * 4] = axis.y * axis.y * omc + c;
			result.elements[2 + 1 * 4] = axis.y * axis.z * omc + axis.x * s;

			result.elements[0 + 2 * 4] = axis.x * axis.z * omc + axis.y * s;
			result.elements[1 + 2 * 4] = axis.y * axis.z * omc - axis.x * s;
			result.elements[2 + 2 * 4] = axis.z * axis.z * omc + c;

			return result;
		}

		Mat4 Mat4::scale(const Vec3& scale)
		{
			Mat4 result(1.0f);

			result.elements[0 + 0 * 4] = scale.x;
			result.elements[1 + 1 * 4] = scale.y;
			result.elements[2 + 2 * 4] = scale.z;

			return result;
		}

		static bool epsilon_equal(float const& x, float const& y)
		{
			return abs(x - y) < std::numeric_limits<float>::epsilon();
		}

		static bool epsilon_not_equal(float const& x, float const& y)
		{
			return abs(x - y) >= std::numeric_limits<float>::epsilon();
		}

		static float length_Vec3(const Vec3& vec)
		{
			return sqrt(Vec3::dot(vec, vec));
		}

		static Vec3 scale_Vec3(const Vec3& vec, float desiredLenght)
		{
			return vec * desiredLenght / length_Vec3(vec);
		}

		bool Mat4::decomposeTransform(const Mat4& transform, Vec3& outPosition, Vec3& outRotation, Vec3& outScale)
		{
			Mat4 localMatix(transform);

			if (epsilon_equal(localMatix.columns[3][3], 0.0f))
			{
				DD_CORE_LOG_ERROR("Could not decompose transform from [{}]", transform); 
				return false;
			}

			// first, isolate perspective.
			if (
				epsilon_not_equal(localMatix.columns[0][3], 0.0f) ||
				epsilon_not_equal(localMatix.columns[1][3], 0.0f) ||
				epsilon_not_equal(localMatix.columns[2][3], 0.0f))
			{
				localMatix.columns[0][3] = localMatix.columns[1][3] = localMatix.columns[2][3] = 0.0f;
				localMatix.columns[3][3] = 1.0f;
			}

			// Next take care of translation
			outPosition = localMatix.columns[3];
			localMatix.columns[3] = Vec4(0, 0, 0, localMatix.columns[3].w);

			Vec3 Row[3];

			// Now get scale and shear
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					Row[i][j] = localMatix.columns[i][j];
				}
			}

			// Compute X scale factor and normalize first row.
			outScale.x = length_Vec3(Row[0]);
			Row[0] = scale_Vec3(Row[0], 1.0f);
			outScale.y = length_Vec3(Row[1]);
			Row[1] = scale_Vec3(Row[1], 1.0f);
			outScale.z = length_Vec3(Row[2]);
			Row[2] = scale_Vec3(Row[2], 1.0f);

			// Issue somewhere in here - maybe a conflic in rads and degrees?

			outRotation.y = asin(-Row[0][2]);
			if (cos(outRotation.y) != 0) {
				outRotation.x = atan2(Row[1][2], Row[2][2]);
				outRotation.z = atan2(Row[0][1], Row[0][0]);
			}
			else {
				outRotation.x = atan2(-Row[2][0], Row[1][1]);
				outRotation.z = 0;
			}

			return true;
		}

} }
