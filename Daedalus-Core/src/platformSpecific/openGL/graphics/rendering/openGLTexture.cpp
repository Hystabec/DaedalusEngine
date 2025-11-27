#include "ddpch.h"
#include "openGLTexture.h"

#include <stb_image.h>
#include <GL/glew.h>

namespace daedalus::graphics {

	namespace utils {

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

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_specification(specification), m_width(m_specification.width), m_height(m_specification.height)
	{
		DD_PROFILE_FUNCTION();

		m_internalFormat = utils::DD_image_format_to_openGL_internal_format(m_specification.format);
		m_dataFormat = utils::DD_image_format_to_openGL_data_format(m_specification.format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& filePath)
		: m_path(filePath)
	{
		DD_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			DD_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& filePath)");
			data = stbi_load(filePath.string().c_str(), &width, &height, &channels, 0);
		}
		DD_CORE_ASSERT(data, DD_ASSERT_FORMAT_MESSAGE("Failed to load image '{}'", filePath));
		m_width = width;
		m_height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;

		DD_CORE_ASSERT(internalFormat & dataFormat, "image format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		DD_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::setData(void* data, uint32_t size)
	{
		DD_PROFILE_FUNCTION();

		DD_CORE_ASSERT(size == m_width * m_height * (m_dataFormat == GL_RGBA ? 4 : 3), "Data must be entire texture");
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		DD_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_rendererID);
	}

}