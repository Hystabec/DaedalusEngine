#pragma once

#include "texture.h"

namespace daedalus::graphics {

	struct MSDFData;

	class Font : public IntrusiveCounter
	{
	public:
		Font(const std::filesystem::path& fontPath);
		~Font();

		const MSDFData* getMSDFData() const { return m_data; }
		IntrusivePtr<Texture2D> getAtlasTexture() const { return m_atlasTexture; };

		static IntrusivePtr<Font> getDefault();
	private:
		MSDFData* m_data;
		IntrusivePtr<Texture2D> m_atlasTexture;
	};

}