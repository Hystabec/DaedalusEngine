#pragma once

#include "graphics/shaders/shader.h"
#include <GL/glew.h>

namespace daedalus::graphics {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::filesystem::path& filePath);
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

		void bind() const override;
		void unbind() const override;

		const std::string& getName() const override { return m_name; }

	private:
		std::unordered_map<GLenum,std::string> preProcess(const std::string& source);

		/// @brief returns true if data has changed
		/// @brief also returns true if the meta file cant be found/read
		bool checkCacheDataChange(const std::string& shaderSrc) const;
		void compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources, bool cacheDataChange);
		void compileOrGetOpenGLBinaries(bool cacheDataChange);
		void createProgram();
		void reflect(GLenum stage, const std::vector<uint32_t>& shaderData);

		GLint getUniformLocation(const std::string& name) const;

	private:
		GLuint m_shaderID;
		std::filesystem::path m_filePath;
		std::string m_name;

		std::unordered_map<GLenum, std::vector<uint32_t>> m_vulkanSPIRV;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_openGLSPIRV;

		std::unordered_map<GLenum, std::string> m_openGLSourceCode;
	};

}