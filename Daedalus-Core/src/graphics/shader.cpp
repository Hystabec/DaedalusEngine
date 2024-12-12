#include "ddpch.h"
#include "shader.h"

#include "utils/fileUtils.h"

namespace daedalusCore { namespace graphics {

		Shader::Shader(const char* vertexPath, const char* fragPath) : m_vertPath(vertexPath), m_fragPath(fragPath)
		{
			m_shaderID = load();
		}

		Shader::~Shader()
		{
			glDeleteProgram(m_shaderID);
		}

		GLint Shader::getUniformLocation(const GLchar* name)
		{
			return glGetUniformLocation(m_shaderID, name);
		}

		void Shader::setUniformMat4(const GLchar* name, const maths::mat4& matrix)
		{
			glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.elements);
		}

		void Shader::setUniform1i(const GLchar* name, int value)
		{
			glUniform1i(getUniformLocation(name), value);
		}

		void Shader::setUniform1iv(const GLchar* name, int* value, int count)
		{
			glUniform1iv(getUniformLocation(name), count, value);
		}

		void Shader::setUniform1f(const GLchar* name, float value)
		{
			glUniform1f(getUniformLocation(name), value);
		}

		void Shader::setUniform1fv(const GLchar* name, float* value, int count)
		{
			glUniform1fv(getUniformLocation(name), count, value);
		}

		void Shader::setUniform2f(const GLchar* name, const maths::vec2& vector)
		{
			glUniform2f(getUniformLocation(name), vector.x, vector.y);
		}

		void Shader::setUniform3f(const GLchar* name, const maths::vec3& vector)
		{
			glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
		}

		void Shader::setUniform4f(const GLchar* name, const maths::vec4& vector)
		{
			glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
		}

		void Shader::enable() const
		{
			glUseProgram(m_shaderID);
		}

		void Shader::disable() const
		{
			glUseProgram(0);
		}

		GLuint Shader::load()
		{
			GLuint program = glCreateProgram();
			
			GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
			GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

			std::string vertSrcS = utils::read_file(m_vertPath);
			std::string fragSrcS = utils::read_file(m_fragPath);
			const char* vertSrc = vertSrcS.c_str();
			const char* fragSrc = fragSrcS.c_str();

			glShaderSource(vertex, 1, &vertSrc, NULL);
			glCompileShader(vertex);

			GLint vertexCompileResult;
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &vertexCompileResult);
			if (vertexCompileResult == GL_FALSE)
			{
				GLint length;
				glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(vertex, length, &length, &error[0]);

				std::cout << "Failed to compile vertex shader" << std::endl << & error[0] << std::endl;
				glDeleteShader(vertex);
				return 0;
			}

			glShaderSource(fragment, 1, &fragSrc, NULL);
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
