#pragma once

namespace daedalus::graphics {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Colour
		RGBA8,
		RED_INTEGER,
		RED_UNSIGNED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: textureFormat(format)
		{
		}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
		// TO DO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: textureSpecifications(attachments)
		{
		}

		std::vector<FramebufferTextureSpecification> textureSpecifications;
	};

	struct FramebufferSpecification
	{
		uint32_t width, height;
		FramebufferAttachmentSpecification attachments;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer : public IntrusiveCounter
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;
		// need to call bind() and unbind() before and after use
		// TO DO: Expand so that a pixel can be read as more types : e.g. int
		virtual uint32_t readPixel(uint32_t attachmentIndex, int x, int y) = 0;

		// TO DO: Expand so that a value can be passed in as more types : e.g. int
		virtual void clearAttachment(uint32_t attachmentIndex, uint32_t value) = 0;

		virtual uint32_t getColourAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;

		static IntrusivePtr<Framebuffer> create(const FramebufferSpecification& specification);
	};

}