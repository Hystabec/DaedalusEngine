#include "ddpch.h"
#include "openGLShader.h"

#include "utils/fileUtils.h"
#include "maths/maths.h"

namespace daedalusCore { namespace graphics {

	static GLenum shaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		DD_CORE_ASSERT(false, debug::Log::formatLogMessage("Unknown shader type: {}", type));
		return 0;
	} 

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		bool wasReadCorrectly = true;
		std::string shaderSrc = utils::read_file(filePath, &wasReadCorrectly);

		if (wasReadCorrectly)
		{
			auto shaderSources = preProcess(shaderSrc);
			m_shaderID = compile(shaderSources);
		}
		else
		{
			DD_CORE_LOG_ERROR("Failed to read shader file : path - {}", filePath);
			m_shaderID = 0; // TO DO: Check if 0 is valid (I dont want it to be)
		}

		// Getting name from file path
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex, const std::string& fragment)
		: m_name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSources{ {GL_VERTEX_SHADER, vertex}, {GL_FRAGMENT_SHADER, fragment} };

		m_shaderID = compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_shaderID);
	}

	void OpenGLShader::setUniformMat4(const maths::mat4& matrix, const std::string& name)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.elements);
	}

	void OpenGLShader::setUniform1i(int value, const std::string& name)
	{
		glUniform1i(getUniformLocation(name), value);
	}

	/*void OpenGLShader::setUniform1iv(int* value, int count, const std::string& name)
	{
		glUniform1iv(getUniformLocation(name), count, value);
	}*/

	void OpenGLShader::setUniform1f(float value, const std::string& name)
	{
		glUniform1f(getUniformLocation(name), value);
	}

	/*void OpenGLShader::setUniform1fv(float* value, int count, const std::string& name)
	{
		glUniform1fv(getUniformLocation(name), count, value);
	}*/

	void OpenGLShader::setUniform2f(const maths::vec2& vector, const std::string& name)
	{
		glUniform2f(getUniformLocation(name), vector.x, vector.y);
	}

	void OpenGLShader::setUniform3f(const maths::vec3& vector, const std::string& name)
	{
		glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void OpenGLShader::setUniform4f(const maths::vec4& vector, const std::string& name)
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

	GLint OpenGLShader::getUniformLocation(const std::string& name)
	{
		return glGetUniformLocation(m_shaderID, name.c_str());
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			DD_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			DD_CORE_ASSERT(shaderTypeFromString(type), "Invalid shader type specifier");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[shaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	GLuint OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();

		DD_CORE_ASSERT(shaderSources.size() <= 3, "Only supports up to 3 shaders");
		std::array<GLenum, 3> glShaderIDs;

		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& src = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* srcCstr = src.c_str();
			glShaderSource(shader, 1, &srcCstr, 0);

			glCompileShader(shader);

			GLint compileResult;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
			if (compileResult == GL_FALSE)
			{
				GLint length;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

				std::vector<GLchar> error(length);
				glGetShaderInfoLog(shader, length, &length, &error[0]);

				DD_CORE_LOG_ERROR("Failed to compile shader | {}", &error[0]);
				//DD_CORE_ASSERT(false, "Shader failed to compile");

				glDeleteShader(shader);
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			/*
			* TO DO: fix vector issue as cant retrive the openGL error message
			* This causes an vector subscript out of range error on glGetProgramInfoLog due to GL_INFO_LOG_LENGTH returning 0
			 
			GLint length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

			
			std::vector<GLchar> infoLog(length);
			glGetProgramInfoLog(program, length, &length, &infoLog[0]);
			

			DD_CORE_LOG_ERROR("Failed to link shader | {}", &infoLog[0]);
			DD_CORE_ASSERT(false, "Shader failed to link");
			*/

			DD_CORE_LOG_ERROR("Failed to link shader");
			DD_CORE_ASSERT(false, "Shader failed to link");

			glDeleteProgram(program);

			while (glShaderIDIndex > 0)
			{
				glDeleteShader(glShaderIDs[glShaderIDIndex-1]);
				glShaderIDIndex--;
			}

			return 0;
		}

		//glValidateProgram(program);

		while (glShaderIDIndex > 0)
		{
			glDetachShader(program, glShaderIDs[glShaderIDIndex-1]);
			glDeleteShader(glShaderIDs[glShaderIDIndex-1]);
			glShaderIDIndex--;
		}

		return program;
	}

} }