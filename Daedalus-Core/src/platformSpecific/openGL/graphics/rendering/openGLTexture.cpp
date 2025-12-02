#include "ddpch.h"
#include "openGLTexture.h"

#include <GL/glew.h>

namespace daedalus::graphics {

	namespace helpers {

		static GLenum DD_image_format_to_openGL_data_format(ImageFormat format)
		{
			switch (format)
			{
			case daedalus::graphics::ImageFormat::RGB8:
				return GL_RGB;
			case daedalus::graphics::ImageFormat::RGBA8:
				return GL_RGBA;
			}

			DD_CORE_ASSERT(false, "Unsupported format");
			return 0;
		}

		static GLenum DD_image_format_to_openGL_internal_format(ImageFormat format)
		{
			switch (format)
			{
			case daedalus::graphics::ImageFormat::RGB8:
				return GL_RGB8;
			case daedalus::graphics::ImageFormat::RGBA8:
				return GL_RGBA8;
			}

			DD_CORE_ASSERT(false, "Unsupported format");
			return 0;
		}

	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification, utils::Buffer data)
		: m_specification(specification), m_width(m_specification.width), m_height(m_specification.height)
	{
		DD_PROFILE_FUNCTION();

		m_internalFormat = helpers::DD_image_format_to_openGL_internal_format(m_specification.format);
		m_dataFormat = helpers::DD_image_format_to_openGL_data_format(m_specification.format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (data)
			setData(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		DD_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::setData(utils::Buffer data)
	{
		DD_PROFILE_FUNCTION();

		uint32_t bytesPerPixel = m_dataFormat == GL_RGBA ? 4 : 3;
		DD_CORE_ASSERT(data.size() == m_width * m_height * bytesPerPixel, "Data must be entire texture");
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data.data());
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		DD_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_rendererID);
	}

}