#pragma once

#include "texture.h"

namespace daedalus::graphics {

	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& fontPath);
		~Font();

		const MSDFData* getMSDFData() const { return m_data; }
		Shr_ptr<Texture2D> getAtlasTexture() const { return m_atlasTexture; };

		static Shr_ptr<Font> getDefault();
	private:
		MSDFData* m_data;
		Shr_ptr<Texture2D> m_atlasTexture;
	};

}