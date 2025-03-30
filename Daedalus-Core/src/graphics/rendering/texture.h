#pragma once

namespace daedalusCore { namespace graphics {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t getWdith() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Texture2D* Create(const std::string& filePath);
	};

} }