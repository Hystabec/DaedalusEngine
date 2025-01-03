#pragma once

namespace daedalusCore { namespace maths {
	struct vec2;
	struct vec3;
	struct vec4;
	struct mat4;
} }

namespace daedalusCore { namespace graphics {

	class Shader
	{
	public:
		virtual ~Shader() {};

		virtual void setUniformMat4(const maths::mat4& matrix, const char* name) = 0;
		virtual void setUniform1i(int value, const char* name) = 0;
		virtual void setUniform1iv(int* value, int count, const char* name) = 0;
		virtual void setUniform1f(float value, const char* name) = 0;
		virtual void setUniform1fv(float* value, int count, const char* name) = 0;
		virtual void setUniform2f(const maths::vec2& vector, const char* name) = 0;
		virtual void setUniform3f(const maths::vec3& vector, const char* name) = 0;
		virtual void setUniform4f(const maths::vec4& vector, const char* name) = 0;

		virtual void enable() const = 0;
		virtual void disable() const = 0;

		static Shader* create(const char* vertex, const char* frag, bool fromFile = true);
	};

} }