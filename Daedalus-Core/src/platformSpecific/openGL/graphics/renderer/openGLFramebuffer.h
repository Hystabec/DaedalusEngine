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
		// need to call bind() and unbind() before and after use
		// TO DO: Expand so that a pixel can be read as more types : e.g. int
		uint32_t readPixel(uint32_t attachmentIndex, int x, int y) override;
		// TO DO: Expand so that a pixel can be read as more types : e.g. int
		void clearAttachment(uint32_t attachmentIndex, uint32_t value) override;

		uint32_t getColourAttachmentRendererID(uint32_t index) const override { DD_CORE_ASSERT(index < m_colourAttachmentIDs.size()); return m_colourAttachmentIDs[index]; }

		const FramebufferSpecification& getSpecification() const override { return m_specification; }
	private:
		uint32_t m_rendererID = 0;
		FramebufferSpecification m_specification;

		std::vector<FramebufferTextureSpecification> m_colourAttachmentSpecification;
		FramebufferTextureSpecification m_depthAttachmentsSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_colourAttachmentIDs;
		uint32_t m_depthAttachmentID = 0;
	};

}