#include "ddpch.h"
#include "openGLShader.h"

#include "utils/fileUtils.h"
#include "maths/maths.h"

namespace daedalus { namespace graphics {

	static GLenum shaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		DD_CORE_ASSERT(false, DD_ASSERT_FORMAT_MESSAGE("Unknown shader type: {}", type));
		return 0;
	} 

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		DD_PROFILE_FUNCTION();

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
		DD_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources{ {GL_VERTEX_SHADER, vertex}, {GL_FRAGMENT_SHADER, fragment} };

		m_shaderID = compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		DD_PROFILE_FUNCTION();

		glDeleteProgram(m_shaderID);
	}

	void OpenGLShader::setUniformMat4(const maths::Mat4& matrix, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.elements);
	}

	void OpenGLShader::setUniform1i(int value, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniform1i(getUniformLocation(name), value);
	}

	void OpenGLShader::setUniformia(int* values, uint32_t count, const std::string& name)
	{
		glUniform1iv(getUniformLocation(name), count, values);
	}

	void OpenGLShader::setUniform1f(float value, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniform1f(getUniformLocation(name), value);
	}

	void OpenGLShader::setUniformfa(float* values, uint32_t count, const std::string& name)
	{
		glUniform1fv(getUniformLocation(name), count, values);
	}

	void OpenGLShader::setUniform2f(const maths::Vec2& vector, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniform2f(getUniformLocation(name), vector.x, vector.y);
	}

	void OpenGLShader::setUniform3f(const maths::Vec3& vector, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void OpenGLShader::setUniform4f(const maths::Vec4& vector, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::enable() const
	{
		DD_PROFILE_FUNCTION();

		glUseProgram(m_shaderID);
	}

	void OpenGLShader::disable() const
	{
		DD_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	GLint OpenGLShader::getUniformLocation(const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		return glGetUniformLocation(m_shaderID, name.c_str());
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& source)
	{
		DD_PROFILE_FUNCTION();

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
		DD_PROFILE_FUNCTION();

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
			GLint length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

			
			std::vector<GLchar> infoLog(length);
			if (length)
			{
				glGetProgramInfoLog(program, length, &length, &infoLog[0]);
				DD_CORE_LOG_ERROR("Failed to link shader | {}", &infoLog[0]);
				DD_CORE_ASSERT(false, "Shader failed to link");
			}
			

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