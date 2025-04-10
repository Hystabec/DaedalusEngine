#include "ddpch.h"
#include "mat4.h"

#include "maths.h"

namespace daedalusCore { namespace maths {

		mat4::mat4()
		{
			memset(elements, 0, sizeof(float) * 16);
		}

		mat4::mat4(float diaganol)
		{
			memset(elements, 0, sizeof(float) * 16);

			elements[0 + 0 * 4] = diaganol;
			elements[1 + 1 * 4] = diaganol;
			elements[2 + 2 * 4] = diaganol;
			elements[3 + 3 * 4] = diaganol;
		}

		mat4::mat4(float* elements)
		{
			memcpy(this->elements, elements, sizeof(float) * 16);
		}

		mat4::mat4(const vec4& column0, const vec4& column1, const vec4& column2, const vec4& column3)
		{
			columns[0] = column0;
			columns[1] = column1;
			columns[2] = column2;
			columns[3] = column3;
		}

		mat4 mat4::identity()
		{
			return mat4(1.0f);
		}

		mat4& mat4::multiply(const mat4& other)
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

		mat4 operator*(mat4 left, const mat4& right)
		{
			return left.multiply(right);
		}

		vec3 operator*(const mat4& left, const vec3& right)
		{
			return left.multiply(right);
		}

		vec4 operator*(const mat4& left, const vec4& right)
		{
			return left.multiply(right);
		}

		mat4& mat4::operator*=(const mat4& other)
		{
			return this->multiply(other);
		}

		vec3 mat4::multiply(const vec3& other) const
		{
			return vec3
			(
				elements[0 + 0 * 4] * other.x + elements[0 + 1 * 4] * other.y + elements[0 + 2 * 4] * other.z + elements[0 + 3 * 4],
				elements[1 + 0 * 4] * other.x + elements[1 + 1 * 4] * other.y + elements[1 + 2 * 4] * other.z + elements[1 + 3 * 4],
				elements[2 + 0 * 4] * other.x + elements[2 + 1 * 4] * other.y + elements[2 + 2 * 4] * other.z + elements[2 + 3 * 4]
			);
		}

		vec4 mat4::multiply(const vec4& other) const
		{
			return vec4
			(
				elements[0 + 0 * 4] * other.x + elements[0 + 1 * 4] * other.y + elements[0 + 2 * 4] * other.z + elements[0 + 3 * 4] * other.w,
				elements[1 + 0 * 4] * other.x + elements[1 + 1 * 4] * other.y + elements[1 + 2 * 4] * other.z + elements[1 + 3 * 4] * other.w,
				elements[2 + 0 * 4] * other.x + elements[2 + 1 * 4] * other.y + elements[2 + 2 * 4] * other.z + elements[2 + 3 * 4] * other.w,
				elements[3 + 0 * 4] * other.x + elements[3 + 1 * 4] * other.y + elements[3 + 2 * 4] * other.z + elements[3 + 3 * 4] * other.w
			);
		}

		mat4& mat4::invert()
		{
			float temp[16];
			memset(temp, 1, sizeof(temp));

			temp[0] = elements[5] * elements[10] * elements[15] -
				elements[5] * elements[11] * elements[14] -
				elements[9] * elements[6] * elements[15] +
				elements[9] * elements[7] * elements[14] +
				elements[13] * elements[6] * elements[11] -
				elements[13] * elements[7] * elements[10];

			temp[4] = -elements[4] * elements[10] * elements[15] +
				elements[4] * elements[11] * elements[14] +
				elements[8] * elements[6] * elements[15] -
				elements[8] * elements[7] * elements[14] -
				elements[12] * elements[6] * elements[11] +
				elements[12] * elements[7] * elements[10];

			temp[8] = elements[4] * elements[9] * elements[15] -
				elements[4] * elements[11] * elements[13] -
				elements[8] * elements[5] * elements[15] +
				elements[8] * elements[7] * elements[13] +
				elements[12] * elements[5] * elements[11] -
				elements[12] * elements[7] * elements[9];

			temp[12] = -elements[4] * elements[9] * elements[14] +
				elements[4] * elements[10] * elements[13] +
				elements[8] * elements[5] * elements[14] -
				elements[8] * elements[6] * elements[13] -
				elements[12] * elements[5] * elements[10] +
				elements[12] * elements[6] * elements[9];

			temp[1] = -elements[1] * elements[10] * elements[15] +
				elements[1] * elements[11] * elements[14] +
				elements[9] * elements[2] * elements[15] -
				elements[9] * elements[3] * elements[14] -
				elements[13] * elements[2] * elements[11] +
				elements[13] * elements[3] * elements[10];

			temp[5] = elements[0] * elements[10] * elements[15] -
				elements[0] * elements[11] * elements[14] -
				elements[8] * elements[2] * elements[15] +
				elements[8] * elements[3] * elements[14] +
				elements[12] * elements[2] * elements[11] -
				elements[12] * elements[3] * elements[10];

			temp[9] = -elements[0] * elements[9] * elements[15] +
				elements[0] * elements[11] * elements[13] +
				elements[8] * elements[1] * elements[15] -
				elements[8] * elements[3] * elements[13] -
				elements[12] * elements[1] * elements[11] +
				elements[12] * elements[3] * elements[9];

			temp[13] = elements[0] * elements[9] * elements[14] -
				elements[0] * elements[10] * elements[13] -
				elements[8] * elements[1] * elements[14] +
				elements[8] * elements[2] * elements[13] +
				elements[12] * elements[1] * elements[10] -
				elements[12] * elements[2] * elements[9];

			temp[2] = elements[1] * elements[6] * elements[15] -
				elements[1] * elements[7] * elements[14] -
				elements[5] * elements[2] * elements[15] +
				elements[5] * elements[3] * elements[14] +
				elements[13] * elements[2] * elements[7] -
				elements[13] * elements[3] * elements[6];

			temp[6] = -elements[0] * elements[6] * elements[15] +
				elements[0] * elements[7] * elements[14] +
				elements[4] * elements[2] * elements[15] -
				elements[4] * elements[3] * elements[14] -
				elements[12] * elements[2] * elements[7] +
				elements[12] * elements[3] * elements[6];

			temp[10] = elements[0] * elements[5] * elements[15] -
				elements[0] * elements[7] * elements[13] -
				elements[4] * elements[1] * elements[15] +
				elements[4] * elements[3] * elements[13] +
				elements[12] * elements[1] * elements[7] -
				elements[12] * elements[3] * elements[5];

			temp[14] = -elements[0] * elements[5] * elements[14] +
				elements[0] * elements[6] * elements[13] +
				elements[4] * elements[1] * elements[14] -
				elements[4] * elements[2] * elements[13] -
				elements[12] * elements[1] * elements[6] +
				elements[12] * elements[2] * elements[5];

			temp[3] = -elements[1] * elements[6] * elements[11] +
				elements[1] * elements[7] * elements[10] +
				elements[5] * elements[2] * elements[11] -
				elements[5] * elements[3] * elements[10] -
				elements[9] * elements[2] * elements[7] +
				elements[9] * elements[3] * elements[6];

			temp[7] = elements[0] * elements[6] * elements[11] -
				elements[0] * elements[7] * elements[10] -
				elements[4] * elements[2] * elements[11] +
				elements[4] * elements[3] * elements[10] +
				elements[8] * elements[2] * elements[7] -
				elements[8] * elements[3] * elements[6];

			temp[11] = -elements[0] * elements[5] * elements[11] +
				elements[0] * elements[7] * elements[9] +
				elements[4] * elements[1] * elements[11] -
				elements[4] * elements[3] * elements[9] -
				elements[8] * elements[1] * elements[7] +
				elements[8] * elements[3] * elements[5];

			temp[15] = elements[0] * elements[5] * elements[10] -
				elements[0] * elements[6] * elements[9] -
				elements[4] * elements[1] * elements[10] +
				elements[4] * elements[2] * elements[9] +
				elements[8] * elements[1] * elements[6] -
				elements[8] * elements[2] * elements[5];

			float determinant = elements[0] * temp[0] + elements[1] * temp[4] + elements[2] * temp[8] + elements[3] * temp[12];
			determinant = (float)(1.0 / determinant);

			for (int i = 0; i < (4 * 4); i++)
				elements[i] = temp[i] * determinant;

			return *this;
		}

		mat4 mat4::invert(const mat4& matrix)
		{
			mat4 result(1.0f);

			result.elements[0] = matrix.elements[5] * matrix.elements[10] * matrix.elements[15] -
				matrix.elements[5] * matrix.elements[11] * matrix.elements[14] -
				matrix.elements[9] * matrix.elements[6] * matrix.elements[15] +
				matrix.elements[9] * matrix.elements[7] * matrix.elements[14] +
				matrix.elements[13] * matrix.elements[6] * matrix.elements[11] -
				matrix.elements[13] * matrix.elements[7] * matrix.elements[10];

			result.elements[4] = -matrix.elements[4] * matrix.elements[10] * matrix.elements[15] +
				matrix.elements[4] * matrix.elements[11] * matrix.elements[14] +
				matrix.elements[8] * matrix.elements[6] * matrix.elements[15] -
				matrix.elements[8] * matrix.elements[7] * matrix.elements[14] -
				matrix.elements[12] * matrix.elements[6] * matrix.elements[11] +
				matrix.elements[12] * matrix.elements[7] * matrix.elements[10];

			result.elements[8] = matrix.elements[4] * matrix.elements[9] * matrix.elements[15] -
				matrix.elements[4] * matrix.elements[11] * matrix.elements[13] -
				matrix.elements[8] * matrix.elements[5] * matrix.elements[15] +
				matrix.elements[8] * matrix.elements[7] * matrix.elements[13] +
				matrix.elements[12] * matrix.elements[5] * matrix.elements[11] -
				matrix.elements[12] * matrix.elements[7] * matrix.elements[9];

			result.elements[12] = -matrix.elements[4] * matrix.elements[9] * matrix.elements[14] +
				matrix.elements[4] * matrix.elements[10] * matrix.elements[13] +
				matrix.elements[8] * matrix.elements[5] * matrix.elements[14] -
				matrix.elements[8] * matrix.elements[6] * matrix.elements[13] -
				matrix.elements[12] * matrix.elements[5] * matrix.elements[10] +
				matrix.elements[12] * matrix.elements[6] * matrix.elements[9];

			result.elements[1] = -matrix.elements[1] * matrix.elements[10] * matrix.elements[15] +
				matrix.elements[1] * matrix.elements[11] * matrix.elements[14] +
				matrix.elements[9] * matrix.elements[2] * matrix.elements[15] -
				matrix.elements[9] * matrix.elements[3] * matrix.elements[14] -
				matrix.elements[13] * matrix.elements[2] * matrix.elements[11] +
				matrix.elements[13] * matrix.elements[3] * matrix.elements[10];

			result.elements[5] = matrix.elements[0] * matrix.elements[10] * matrix.elements[15] -
				matrix.elements[0] * matrix.elements[11] * matrix.elements[14] -
				matrix.elements[8] * matrix.elements[2] * matrix.elements[15] +
				matrix.elements[8] * matrix.elements[3] * matrix.elements[14] +
				matrix.elements[12] * matrix.elements[2] * matrix.elements[11] -
				matrix.elements[12] * matrix.elements[3] * matrix.elements[10];

			result.elements[9] = -matrix.elements[0] * matrix.elements[9] * matrix.elements[15] +
				matrix.elements[0] * matrix.elements[11] * matrix.elements[13] +
				matrix.elements[8] * matrix.elements[1] * matrix.elements[15] -
				matrix.elements[8] * matrix.elements[3] * matrix.elements[13] -
				matrix.elements[12] * matrix.elements[1] * matrix.elements[11] +
				matrix.elements[12] * matrix.elements[3] * matrix.elements[9];

			result.elements[13] = matrix.elements[0] * matrix.elements[9] * matrix.elements[14] -
				matrix.elements[0] * matrix.elements[10] * matrix.elements[13] -
				matrix.elements[8] * matrix.elements[1] * matrix.elements[14] +
				matrix.elements[8] * matrix.elements[2] * matrix.elements[13] +
				matrix.elements[12] * matrix.elements[1] * matrix.elements[10] -
				matrix.elements[12] * matrix.elements[2] * matrix.elements[9];

			result.elements[2] = matrix.elements[1] * matrix.elements[6] * matrix.elements[15] -
				matrix.elements[1] * matrix.elements[7] * matrix.elements[14] -
				matrix.elements[5] * matrix.elements[2] * matrix.elements[15] +
				matrix.elements[5] * matrix.elements[3] * matrix.elements[14] +
				matrix.elements[13] * matrix.elements[2] * matrix.elements[7] -
				matrix.elements[13] * matrix.elements[3] * matrix.elements[6];

			result.elements[6] = -matrix.elements[0] * matrix.elements[6] * matrix.elements[15] +
				matrix.elements[0] * matrix.elements[7] * matrix.elements[14] +
				matrix.elements[4] * matrix.elements[2] * matrix.elements[15] -
				matrix.elements[4] * matrix.elements[3] * matrix.elements[14] -
				matrix.elements[12] * matrix.elements[2] * matrix.elements[7] +
				matrix.elements[12] * matrix.elements[3] * matrix.elements[6];

			result.elements[10] = matrix.elements[0] * matrix.elements[5] * matrix.elements[15] -
				matrix.elements[0] * matrix.elements[7] * matrix.elements[13] -
				matrix.elements[4] * matrix.elements[1] * matrix.elements[15] +
				matrix.elements[4] * matrix.elements[3] * matrix.elements[13] +
				matrix.elements[12] * matrix.elements[1] * matrix.elements[7] -
				matrix.elements[12] * matrix.elements[3] * matrix.elements[5];

			result.elements[14] = -matrix.elements[0] * matrix.elements[5] * matrix.elements[14] +
				matrix.elements[0] * matrix.elements[6] * matrix.elements[13] +
				matrix.elements[4] * matrix.elements[1] * matrix.elements[14] -
				matrix.elements[4] * matrix.elements[2] * matrix.elements[13] -
				matrix.elements[12] * matrix.elements[1] * matrix.elements[6] +
				matrix.elements[12] * matrix.elements[2] * matrix.elements[5];

			result.elements[3] = -matrix.elements[1] * matrix.elements[6] * matrix.elements[11] +
				matrix.elements[1] * matrix.elements[7] * matrix.elements[10] +
				matrix.elements[5] * matrix.elements[2] * matrix.elements[11] -
				matrix.elements[5] * matrix.elements[3] * matrix.elements[10] -
				matrix.elements[9] * matrix.elements[2] * matrix.elements[7] +
				matrix.elements[9] * matrix.elements[3] * matrix.elements[6];

			result.elements[7] = matrix.elements[0] * matrix.elements[6] * matrix.elements[11] -
				matrix.elements[0] * matrix.elements[7] * matrix.elements[10] -
				matrix.elements[4] * matrix.elements[2] * matrix.elements[11] +
				matrix.elements[4] * matrix.elements[3] * matrix.elements[10] +
				matrix.elements[8] * matrix.elements[2] * matrix.elements[7] -
				matrix.elements[8] * matrix.elements[3] * matrix.elements[6];

			result.elements[11] = -matrix.elements[0] * matrix.elements[5] * matrix.elements[11] +
				matrix.elements[0] * matrix.elements[7] * matrix.elements[9] +
				matrix.elements[4] * matrix.elements[1] * matrix.elements[11] -
				matrix.elements[4] * matrix.elements[3] * matrix.elements[9] -
				matrix.elements[8] * matrix.elements[1] * matrix.elements[7] +
				matrix.elements[8] * matrix.elements[3] * matrix.elements[5];

			result.elements[15] = matrix.elements[0] * matrix.elements[5] * matrix.elements[10] -
				matrix.elements[0] * matrix.elements[6] * matrix.elements[9] -
				matrix.elements[4] * matrix.elements[1] * matrix.elements[10] +
				matrix.elements[4] * matrix.elements[2] * matrix.elements[9] +
				matrix.elements[8] * matrix.elements[1] * matrix.elements[6] -
				matrix.elements[8] * matrix.elements[2] * matrix.elements[5];

			float determinant = matrix.elements[0] * result.elements[0] + matrix.elements[1] * result.elements[4] + matrix.elements[2] * result.elements[8] + matrix.elements[3] * result.elements[12];
			determinant = (float)(1.0 / determinant);

			for (int i = 0; i < (4 * 4); i++)
				result.elements[i] = result.elements[i] * determinant;

			return result;
		}

		mat4 mat4::orthographic(float left, float right, float botton, float top, float nearPlane, float farPlane)
		{
			mat4 result(1.0f);

			result.elements[0 + 0 * 4] = 2.0f / (right - left);
			result.elements[1 + 1 * 4] = 2.0f / (top - botton);
			result.elements[2 + 2 * 4] = 2.0f / (nearPlane - farPlane);

			result.elements[0 + 3 * 4] = (left + right) / (left - right);
			result.elements[1 + 3 * 4] = (botton + top) / (botton - top);
			result.elements[2 + 3 * 4] = (farPlane + nearPlane) / (farPlane - nearPlane);

			return result;
		}

		mat4 mat4::perspective(float fov, float aspectRatio, float nearPlane, float farPlane)
		{
			mat4 result(1.0f);
			
			float q = 1.0f / tan(degrees_to_radians(fov / 2));
			float a = q / aspectRatio;
			
			result.elements[0 + 0 * 4] = a;
			result.elements[1 + 1 * 4] = q;
			result.elements[2 + 2 * 4] = ((nearPlane + farPlane) / (nearPlane - farPlane));
			result.elements[3 + 2 * 4] = -1.0f;
			result.elements[2 + 3 * 4] = ((2.0f * nearPlane * farPlane) / (nearPlane - farPlane));

			return result;
		}

		mat4 mat4::translate(const vec3& translation)
		{
			mat4 result(1.0f);

			result.elements[0 + 3 * 4] = translation.x;
			result.elements[1 + 3 * 4] = translation.y;
			result.elements[2 + 3 * 4] = translation.z;

			return result;
		}

		mat4 mat4::rotate(float angle, const vec3& axis, bool inRadians)
		{
			mat4 result(1.0f);

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

		mat4 mat4::scale(const vec3& scale)
		{
			mat4 result(1.0f);

			result.elements[0 + 0 * 4] = scale.x;
			result.elements[1 + 1 * 4] = scale.y;
			result.elements[2 + 2 * 4] = scale.z;

			return result;
		}

} }
