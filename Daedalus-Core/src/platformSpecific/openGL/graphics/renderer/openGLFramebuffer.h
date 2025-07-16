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