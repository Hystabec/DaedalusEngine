#pragma once

#include "graphics/shaders/shader.h"
#include <GL/glew.h>

namespace daedalusCore { namespace graphics {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertex, const std::string& fragment);

		virtual ~OpenGLShader();

		void setUniformMat4(const maths::mat4& matrix, const std::string& name) override;
		void setUniform1i(int value, const std::string& name) override;
		//void setUniform1iv(int* value, int count, const GLchar* name) override;
		void setUniform1f(float value, const std::string& name) override;
		//void setUniform1fv(float* value, int count, const GLchar* name) override;
		void setUniform2f(const maths::vec2& vector, const std::string& name) override;
		void setUniform3f(const maths::vec3& vector, const std::string& name) override;
		void setUniform4f(const maths::vec4& vector, const std::string& name) override;

		void enable() const override;
		void disable() const override;

		const std::string& getName() const override { return m_name; }

	private:
		GLint getUniformLocation(const std::string& name);

		std::unordered_map<GLenum,std::string> preProcess(const std::string& source);
		GLuint compile(const std::unordered_map<GLenum, std::string>& shaderSources);

		std::string m_name;
	private:
		GLuint m_shaderID;
	};

} }