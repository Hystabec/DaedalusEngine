#pragma once

#include "graphics/rendering/texture.h"
#include <GL/glew.h>

namespace daedalusCore { namespace graphics {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& filePath);
		~OpenGLTexture2D();

		uint32_t getWdith() const override { return m_width; }
		uint32_t getHeight() const  override {return m_height; }
		uint32_t getRendererID() const override { return m_rendererID; }

		void setData(void* data, uint32_t size) override;

		void bind(uint32_t slot = 0) const override;

		bool operator==(const Texture& other) const override 
		{
			return m_rendererID == ((OpenGLTexture2D&)other).m_rendererID;
		};

	private:
		std::string m_path;

		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_rendererID;
		GLenum m_internalFormat, m_dataFormat;
	};

} }