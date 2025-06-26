#pragma once

namespace daedalus::graphics {

	struct FramebufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t getColourAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;

		static Shr_ptr<Framebuffer> create(const FramebufferSpecification& specification);
	};

}