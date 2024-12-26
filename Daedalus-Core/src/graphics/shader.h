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

		virtual void setUniformMat4(const char* name, const maths::mat4& matrix) = 0;
		virtual void setUniform1i(const char* name, int value) = 0;
		virtual void setUniform1iv(const char* name, int* value, int count) = 0;
		virtual void setUniform1f(const char* name, float value) = 0;
		virtual void setUniform1fv(const char* name, float* value, int count) = 0;
		virtual void setUniform2f(const char* name, const maths::vec2& vector) = 0;
		virtual void setUniform3f(const char* name, const maths::vec3& vector) = 0;
		virtual void setUniform4f(const char* name, const maths::vec4& vector) = 0;

		virtual void enable() const = 0;
		virtual void disable() const = 0;

		static Shader* create(const char* vertexPath, const char* fragPath);
	};

} }