#pragma once

#include <GL/glew.h>
#include "../maths/maths.h"

namespace daedalusCore { namespace graphics {

	class Shader
	{
	private:
		GLuint m_shaderID;
		const char* m_vertPath, *m_fragPath;

	public:
		Shader(const char* vertexPath, const char* fragPath);
		~Shader();

		void setUniformMat4(const GLchar* name, const maths::mat4& matrix);
		void setUniform1i(const GLchar* name, int value);
		void setUniform1f(const GLchar* name, float value);
		void setUniform2f(const GLchar* name, const maths::vec2& vector);
		void setUniform3f(const GLchar* name, const maths::vec3& vector);
		void setUniform4f(const GLchar* name, const maths::vec4& vector);

		void enable() const;
		void disable() const;

	private:
		GLuint load();
		GLint getUniformLocation(const GLchar* name);
	};

} }