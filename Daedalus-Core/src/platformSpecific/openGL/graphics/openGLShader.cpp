#include "ddpch.h"
#include "openGLShader.h"

#include "utils/fileUtils.h"
#include "maths/maths.h"

namespace daedalusCore { namespace graphics {

	OpenGLShader::OpenGLShader(const GLchar* vertex, const GLchar* fragment)
	{
		m_shaderID = load(vertex, fragment);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_shaderID);
	}

	void OpenGLShader::setUniformMat4(const maths::mat4& matrix, const GLchar* name)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.elements);
	}

	void OpenGLShader::setUniform1i(int value, const GLchar* name)
	{
		glUniform1i(getUniformLocation(name), value);
	}

	void OpenGLShader::setUniform1iv(int* value, int count, const GLchar* name)
	{
		glUniform1iv(getUniformLocation(name), count, value);
	}

	void OpenGLShader::setUniform1f(float value, const GLchar* name)
	{
		glUniform1f(getUniformLocation(name), value);
	}

	void OpenGLShader::setUniform1fv(float* value, int count, const GLchar* name)
	{
		glUniform1fv(getUniformLocation(name), count, value);
	}

	void OpenGLShader::setUniform2f(const maths::vec2& vector, const GLchar* name)
	{
		glUniform2f(getUniformLocation(name), vector.x, vector.y);
	}

	void OpenGLShader::setUniform3f(const maths::vec3& vector, const GLchar* name)
	{
		glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void OpenGLShader::setUniform4f(const maths::vec4& vector, const GLchar* name)
	{
		glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::enable() const
	{
		glUseProgram(m_shaderID);
	}

	void OpenGLShader::disable() const
	{
		glUseProgram(0);
	}

	GLint OpenGLShader::getUniformLocation(const GLchar* name)
	{
		return glGetUniformLocation(m_shaderID, name);
	}

	GLuint OpenGLShader::load(const GLchar* vert, const GLchar* frag)
	{
		GLuint program = glCreateProgram();

		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);


		glShaderSource(vertex, 1, &vert, NULL);
		glCompileShader(vertex);

		GLint vertexCompileResult;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &vertexCompileResult);
		if (vertexCompileResult == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(vertex, length, &length, &error[0]);

			std::cout << "Failed to compile vertex shader" << std::endl << &error[0] << std::endl;
			glDeleteShader(vertex);
			return 0;
		}

		glShaderSource(fragment, 1, &frag, NULL);
		glCompileShader(fragment);

		GLint fragmentCompileResult;
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &fragmentCompileResult);
		if (fragmentCompileResult == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(fragment, length, &length, &error[0]);
			std::cout << "Failed to compile fragment shader" << std::endl << &error[0] << std::endl;
			glDeleteShader(fragment);
			return 0;
		}

		glAttachShader(program, vertex);
		glAttachShader(program, fragment);

		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return program;
	}

} }