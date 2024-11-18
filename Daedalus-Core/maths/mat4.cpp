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

		mat4 mat4::identity()
		{
			return mat4(1.0f);
		}

		mat4& mat4::multiply(const mat4& other)
		{
			float data[16];
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					float sum = 0.0f;
					for (int elm = 0; elm < 4; elm++)
					{
						sum += elements[x + elm * 4] * other.elements[elm + y * 4];
					}
					data[x + y * 4] = sum;
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

		mat4 mat4::orthographic(float left, float right, float botton, float top, float near, float far)
		{
			mat4 result(1.0f);

			result.elements[0 + 0 * 4] = 2.0f / (right - left);
			result.elements[1 + 1 * 4] = 2.0f / (top - botton);
			result.elements[2 + 2 * 4] = 2.0f / (near - far);

			result.elements[0 + 3 * 4] = (left + right) / (left - right);
			result.elements[1 + 3 * 4] = (botton + top) / (botton - top);
			result.elements[2 + 3 * 4] = (far + near) / (far - near);

			return result;
		}

		mat4 mat4::perspective(float fov, float aspectRatio, float near, float far)
		{
			mat4 result(1.0f);
			
			float q = 1.0f / tan(degreesToRadians(fov / 2));
			float a = q / aspectRatio;
			
			result.elements[0 + 0 * 4] = a;
			result.elements[1 + 1 * 4] = q;
			result.elements[2 + 2 * 4] = ((near + far) / (near - far));
			result.elements[3 + 2 * 4] = -1.0f;
			result.elements[2 + 3 * 4] = ((2 * near * far) / (near - far));

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

		mat4 mat4::rotate(float angle, const vec3& axis)
		{
			mat4 result(1.0f);

			float asRads = degreesToRadians(angle);
			float c = cos(asRads);
			float s = sin(asRads);
			float mc = 1.0f - c;

			result.elements[0 + 0 * 4] = axis.x * mc + c;
			result.elements[1 + 0 * 4] = axis.y * axis.x * mc + axis.z * s;
			result.elements[2 + 0 * 4] = axis.x * axis.z * mc - axis.y * s;

			result.elements[0 + 1 * 4] = axis.x * axis.y * mc - axis.z * s;
			result.elements[1 + 1 * 4] = axis.y * mc + c;
			result.elements[2 + 1 * 4] = axis.y * axis.z * mc + axis.x * s;

			result.elements[0 + 2 * 4] = axis.x * axis.z * mc + axis.y * s;
			result.elements[1 + 2 * 4] = axis.y * axis.z * mc - axis.x * s;
			result.elements[2 + 2 * 4] = axis.z * mc + c;

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
