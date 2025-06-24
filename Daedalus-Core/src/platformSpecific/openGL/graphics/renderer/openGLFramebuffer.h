#pragma once

#include "graphics/renderer/framebuffer.h"

namespace daedalus::graphics {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer() override;

		void invalidate();

		void bind() override;
		void unbind() override;

		void resize(uint32_t width, uint32_t height) override;

		uint32_t getColourAttachmentRendererID() const override { return m_colourAttachment; }

		const FramebufferSpecification& getSpecification() const override { return m_specification; }
	private:
		uint32_t m_rendererID = 0;
		uint32_t m_colourAttachment = 0, m_depthAttachment = 0;
		FramebufferSpecification m_specification;
	};

}