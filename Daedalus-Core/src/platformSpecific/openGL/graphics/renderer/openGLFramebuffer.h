#pragma once

#include "graphics/renderer/framebuffer.h"

namespace daedalus::graphics {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		void invalidate();

		void bind() override;
		void unbind() override;

		uint32_t getColourAttachmentRendererID() const override { return m_colourAttachment; }

		const FramebufferSpecification& getSpecification() const override { return m_specification; }
	private:
		uint32_t m_rendererID;
		uint32_t m_colourAttachment, m_depthAttachment;
		FramebufferSpecification m_specification;
	};

}