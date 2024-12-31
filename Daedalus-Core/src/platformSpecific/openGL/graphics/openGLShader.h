#pragma once

#include "graphics/shader.h"
#include <GL/glew.h>

namespace daedalusCore { namespace graphics {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const GLchar* vertex, const GLchar* fragment);
		virtual ~OpenGLShader();

		void setUniformMat4(const GLchar* name, const maths::mat4& matrix) override;
		void setUniform1i(const GLchar* name, int value) override;
		void setUniform1iv(const GLchar* name, int* value, int count) override;
		void setUniform1f(const GLchar* name, float value) override;
		void setUniform1fv(const GLchar* name, float* value, int count) override;
		void setUniform2f(const GLchar* name, const maths::vec2& vector) override;
		void setUniform3f(const GLchar* name, const maths::vec3& vector) override;
		void setUniform4f(const GLchar* name, const maths::vec4& vector) override;

		void enable() const override;
		void disable() const override;

	private:
		GLint getUniformLocation(const GLchar* name);
		GLuint load(const GLchar* vertex, const GLchar* fragment);

	private:
		GLuint m_shaderID;
	};

} }