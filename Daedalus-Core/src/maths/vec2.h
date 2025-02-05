#pragma once

namespace daedalusCore { namespace maths {

	struct vec2
	{
		float x, y;

		vec2();
		vec2(const float& x);
		vec2(const float& x, const float& y);

		vec2& add(const vec2& other);
		vec2& subtract(const vec2& other);
		vec2& multiply(const vec2& other);
		vec2& divide(const vec2& other);

		friend vec2 operator +(vec2 left, const vec2& right);
		friend vec2 operator -(vec2 left, const vec2& right);
		friend vec2 operator *(vec2 left, const vec2& right);
		friend vec2 operator /(vec2 left, const vec2& right);
		
		bool operator==(const vec2& other);
		bool operator!=(const vec2& other);

		vec2& operator +=(const vec2& other);
		vec2& operator -=(const vec2& other);
		vec2& operator *=(const vec2& other);
		vec2& operator /=(const vec2& other);

		friend std::ostream& operator<<(std::ostream& stream, const vec2& vector);
	};

} }

LOG_CREATE_FORMAT(daedalusCore::maths::vec2, "x: {}, y: {}", vec, vec.x, vec.y);