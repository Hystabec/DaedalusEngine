#pragma once

#include "graphics/shaders/shader.h"
#include <GL/glew.h>

namespace daedalus { namespace graphics {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertex, const std::string& fragment);

		virtual ~OpenGLShader();

		void setUniformMat4(const maths::Mat4& matrix, const std::string& name) override;
		void setUniform1i(int value, const std::string& name) override;
		void setUniformia(int* values, uint32_t count, const std::string& name) override;
		void setUniform1f(float value, const std::string& name) override;
		void setUniformfa(float* values, uint32_t count, const std::string& name) override;
		void setUniform2f(const maths::Vec2& vector, const std::string& name) override;
		void setUniform3f(const maths::Vec3& vector, const std::string& name) override;
		void setUniform4f(const maths::Vec4& vector, const std::string& name) override;

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