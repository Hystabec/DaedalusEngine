#pragma once

#include "graphics/rendering/texture.h"

namespace daedalusCore { namespace graphics {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filePath);
		~OpenGLTexture2D();

		uint32_t getWdith() const override { return m_width; }
		uint32_t getHeight() const  override {return m_height; }

		void bind(uint32_t slot = 0) const override;

	private:
		std::string m_path;

		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_rendererID;
	};

} }