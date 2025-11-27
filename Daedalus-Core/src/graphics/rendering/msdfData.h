#pragma once

#include <vector>

// Undef INFINITE or there could be a clash with windows.h
#undef INFINITE
#include <msdf-atlas-gen.h>

namespace daedalus::graphics {

	struct MSDFData
	{
		std::vector<msdf_atlas::GlyphGeometry> glyphs;
		msdf_atlas::FontGeometry fontGeometry;
	};

}