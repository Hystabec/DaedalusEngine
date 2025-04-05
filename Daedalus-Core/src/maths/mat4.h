#pragma once

#include "vec3.h"
#include "vec4.h"

namespace daedalusCore { namespace maths {

	struct mat4
	{
		//column-major

		union 
		{
			float elements[16];
			vec4 columns[4];
		};

		mat4();
		mat4(float diagonal);
		mat4(float* elements);
		mat4(const vec4& column0, const vec4& column1, const vec4& column2, const vec4& column3);

		static mat4 identity();
		mat4& multiply(const mat4& other);
		friend mat4 operator*(mat4 left, const mat4& right);
		mat4& operator*=(const mat4& other);

		vec3 multiply(const vec3& other) const;
		friend vec3 operator*(const mat4& left, const vec3& right);

		vec4 multiply(const vec4& other) const;
		friend vec4 operator*(const mat4& left, const vec4& right);

		mat4& invert();
		static mat4 invert(const mat4& matrix);

		static mat4 orthographic(float left, float right, float botton, float top, float nearPlane, float farPlane);
		static mat4 perspective(float fov, float aspectRatio, float nearPlane, float farPlane);

		static mat4 translate(const vec3& translation);
		static mat4 rotate(float angle, const vec3& axis);
		static mat4 scale(const vec3& scale);
	};

} }

LOG_CREATE_FORMAT(daedalusCore::maths::mat4, "{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}", 
	thisMat4, 
	thisMat4.elements[0], thisMat4.elements[1], thisMat4.elements[2], thisMat4.elements[3], 
	thisMat4.elements[4], thisMat4.elements[5], thisMat4.elements[6], thisMat4.elements[7],
	thisMat4.elements[8], thisMat4.elements[9], thisMat4.elements[10], thisMat4.elements[11],
	thisMat4.elements[12], thisMat4.elements[13], thisMat4.elements[14], thisMat4.elements[15])