#include "ddpch.h"
#include "openGLFramebuffer.h"

#include <GL/glew.h>

namespace daedalus::graphics {

	static const uint32_t s_maxFramebufferSize = 8192;

	namespace helpers
	{
		static GLenum texture_target(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void create_textures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(texture_target(multisampled), count, outID);
		}

		static void bind_texture(bool multisampled, uint32_t id)
		{
			glBindTexture(texture_target(multisampled), id);
		}

		static void attach_colour_texture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture_target(multisampled), id, 0);
		}

		static void attach_depth_texture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, texture_target(multisampled), id, 0);
		}

		static bool is_depth_format(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				return true;
			}

			return false;
		}

		static GLenum DD_FB_texture_format_to_GL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case daedalus::graphics::FramebufferTextureFormat::RGBA8:
				return GL_RGBA8;
			case daedalus::graphics::FramebufferTextureFormat::RED_INTEGER:
			case daedalus::graphics::FramebufferTextureFormat::RED_UNSIGNED_INTEGER: // the is no internal openGL GL_RED_UNSIGNED_INTEGER
				return GL_RED_INTEGER;
			}

			DD_CORE_ASSERT(false, "Unknown format");
			return 0;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
		: m_specification(specification)
	{
		for (auto& spec : m_specification.attachments.textureSpecifications)
		{
			if (!helpers::is_depth_format(spec.textureFormat))
				m_colourAttachmentSpecification.emplace_back(spec);
			else
				m_depthAttachmentsSpecification = spec;
		}

		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures((GLsizei)m_colourAttachmentIDs.size(), m_colourAttachmentIDs.data());
		glDeleteTextures(1, &m_depthAttachmentID);
	}

	void OpenGLFramebuffer::invalidate()
	{
		if (m_rendererID)
		{
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures((GLsizei)m_colourAttachmentIDs.size(), m_colourAttachmentIDs.data());
			glDeleteTextures(1, &m_depthAttachmentID);

			m_colourAttachmentIDs.clear();
			m_depthAttachmentID = 0;
		}

		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		// Attachments
		bool multisample = m_specification.samples > 1;

		if (m_colourAttachmentSpecification.size())
		{
			m_colourAttachmentIDs.resize(m_colourAttachmentSpecification.size());
			helpers::create_textures(multisample, m_colourAttachmentIDs.data(), (uint32_t)m_colourAttachmentIDs.size());

			for (size_t i = 0; i < m_colourAttachmentIDs.size(); i++)
			{
				helpers::bind_texture(multisample, m_colourAttachmentIDs[i]);
				switch (m_colourAttachmentSpecification[i].textureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					helpers::attach_colour_texture(m_colourAttachmentIDs[i], m_specification.samples, GL_RGBA8, GL_RGBA, m_specification.width, m_specification.height, (int)i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					helpers::attach_colour_texture(m_colourAttachmentIDs[i], m_specification.samples, GL_R32I, GL_RED_INTEGER, m_specification.width, m_specification.height, (int)i);
					break;
				case FramebufferTextureFormat::RED_UNSIGNED_INTEGER:
					helpers::attach_colour_texture(m_colourAttachmentIDs[i], m_specification.samples, GL_R32UI, GL_RED_INTEGER, m_specification.width, m_specification.height, (int)i);
					break;
				}
			}
		}

		if (m_depthAttachmentsSpecification.textureFormat != FramebufferTextureFormat::None)
		{
			helpers::create_textures(multisample, &m_depthAttachmentID, 1);
			helpers::bind_texture(multisample, m_depthAttachmentID);
			switch (m_depthAttachmentsSpecification.textureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				helpers::attach_depth_texture(m_depthAttachmentID, m_specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_specification.width, m_specification.height);
				break;
			}
		}

		if (m_colourAttachmentIDs.size() > 1)
		{
			DD_CORE_ASSERT(m_colourAttachmentIDs.size() <= 4, "Currently only supports up to 4 colour attachments");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,  GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_colourAttachmentIDs.size(), buffers);
		}
		else if (m_colourAttachmentIDs.empty())
		{
			// only depth-pass
			glDrawBuffer(GL_NONE);
		}

		DD_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_specification.width, m_specification.height);
	}

	void OpenGLFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_maxFramebufferSize || height > s_maxFramebufferSize)
		{
			DD_CORE_LOG_WARN("Attempted to resize framebuffer to {}, {}", width, height);
			return;
		}

		m_specification.width = width;
		m_specification.height = height;

		invalidate();
	}

	uint32_t OpenGLFramebuffer::readPixel(uint32_t attachmentIndex, int x, int y)
	{
		DD_CORE_ASSERT(attachmentIndex < m_colourAttachmentIDs.size());

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		uint32_t pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixelData);

		return pixelData;
	}

	void OpenGLFramebuffer::clearAttachment(uint32_t attachmentIndex, uint32_t value)
	{
		DD_CORE_ASSERT(attachmentIndex < m_colourAttachmentIDs.size());

		auto& spec = m_colourAttachmentSpecification[attachmentIndex];

		glClearTexImage(m_colourAttachmentIDs[attachmentIndex], 0,
			helpers::DD_FB_texture_format_to_GL(spec.textureFormat), GL_UNSIGNED_INT, &value);
	}

}