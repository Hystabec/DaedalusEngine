#pragma once

#include <format>

/*
Use to allow the logger to output custom types
Use in global namespace

Parms:
type = The Type to create format for | e.g.Vec2
fmt  = The format to display | e.g. "x.{}, y.{}"
name = A name for the object | e.g. vec
...  = Any arguments for the format (with (name). infront) | e.g. vec.x, vec.y
*/
#define LOG_CREATE_FORMAT(type, fmt, name, ...)	template<> \
struct std::formatter<type> {\
constexpr auto parse(std::format_parse_context& ctx)\
{\
	return ctx.begin();\
}\
	auto format(const type& name, std::format_context& ctx) const\
	{\
		return std::format_to(ctx.out(), fmt, ##__VA_ARGS__);\
	}\
};

/*
Example:

namespace math
{
	class Vec2
	{
	private:
		int z;

	public:
		int x, y;

		Vec2(int x, int y, int z)
			: x(x), y(y), z(z)
		{
		}

		inline int GetZ() const { return z; }
	}
}

TestFormatMacro(Vec2, "x: {}, y:{}, z: {}", vec, vec.x, vec.y, vec.GetZ())

*/