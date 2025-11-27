#pragma once

#include "graphics/rendering/texture.h"
#include <GL/glew.h>

namespace daedalus::graphics {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureSpecification& specification);
		OpenGLTexture2D(const std::filesystem::path& filePath);
		~OpenGLTexture2D();

		const TextureSpecification& getSpecification() const override { return m_specification; }

		uint32_t getWdith() const override { return m_width; }
		uint32_t getHeight() const  override {return m_height; }
		uint32_t getRendererID() const override { return m_rendererID; }

		const std::filesystem::path& getSrcFilePath() const { return m_path; };

		void setData(void* data, uint32_t size) override;

		void bind(uint32_t slot = 0) const override;

		bool operator==(const Texture& other) const override 
		{
			return m_rendererID == ((OpenGLTexture2D&)other).m_rendererID;
		};

	private:
		TextureSpecification m_specification;

		std::filesystem::path m_path;

		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_rendererID;
		GLenum m_internalFormat, m_dataFormat;
	};

}