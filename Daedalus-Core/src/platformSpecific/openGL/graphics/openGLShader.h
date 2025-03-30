#pragma once

#include "graphics/shader.h"
#include <GL/glew.h>

namespace daedalusCore { namespace graphics {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const GLchar* vertex, const GLchar* fragment);
		virtual ~OpenGLShader();

		void setUniformMat4(const maths::mat4& matrix, const GLchar* name) override;
		void setUniform1i(int value, const GLchar* name) override;
		//void setUniform1iv(int* value, int count, const GLchar* name) override;
		void setUniform1f(float value, const GLchar* name) override;
		//void setUniform1fv(float* value, int count, const GLchar* name) override;
		void setUniform2f(const maths::vec2& vector, const GLchar* name) override;
		void setUniform3f(const maths::vec3& vector, const GLchar* name) override;
		void setUniform4f(const maths::vec4& vector, const GLchar* name) override;

		void enable() const override;
		void disable() const override;

	private:
		GLint getUniformLocation(const GLchar* name);
		GLuint load(const GLchar* vertex, const GLchar* fragment);

	private:
		GLuint m_shaderID;
	};

} }