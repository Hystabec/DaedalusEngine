#include "ddpch.h"
#include "font.h"

#include "texture.h"
#include "utils/findFileLocation.h"
#include "msdfData.h"

// Undef INFINITE or there could be a clash with windows.h
#undef INFINITE
#include <msdf-atlas-gen.h>
#include <msdfgen.h>
#include <FontGeometry.h>

namespace daedalus::graphics {

	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static Shr_ptr<Texture2D> create_and_cache_atlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs, const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
	{
		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		generator.setAttributes(attributes);
		// NOTE: 8 threads is slightly random. Could check how many threads are avalible and used half or something like that
		generator.setThreadCount(8);
		generator.generate(glyphs.data(), (int)glyphs.size());
		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

		TextureSpecification spec;
		spec.width = bitmap.width;
		spec.height = bitmap.height;
		spec.format = ImageFormat::RGB8;
		spec.generateMips = false;

		Shr_ptr<Texture2D> texture = Texture2D::create(spec);
		texture->setData((void*)bitmap.pixels, bitmap.width * bitmap.height * 3);
		return texture;
		// TO DO: Add cahcing for the texture atlas so that they dont need to be regenerated each time
	}

	Font::Font(const std::filesystem::path& fontPath)
		: m_data(new MSDFData())
	{
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		DD_CORE_ASSERT(ft);

		// TO DO: msdfgen::loadFontData loads from memory buffer. This will be need to read a font's data
		// when they have been packaged for distro
		msdfgen::FontHandle* font = loadFont(ft, fontPath.string().c_str());
		if (!font) {
			DD_CORE_LOG_ERROR("Failed to load font: {}", fontPath);
			return;
		}

		struct CharsetRange
		{
			uint32_t begin, end;
		};

		static const CharsetRange charsetRanges[] =
		{
			// From imgui_draw.cpp ( line number:3382 (as of 26.11.25) )
			{ 0x0020, 0x00FF } // Basic Latin + Latin Supplement
		};

		msdf_atlas::Charset charset;
		for (CharsetRange range : charsetRanges)
		{
			for (uint32_t c = range.begin; c <= range.end; c++)
				charset.add(c);
		}

		double fontScale = 1.0;
		m_data->fontGeometry = msdf_atlas::FontGeometry(&m_data->glyphs);
		int glyphsLoaded = m_data->fontGeometry.loadCharset(font, fontScale, charset);
		DD_CORE_LOG_INFO("Loaded {}/{} glyphs from font: {}", glyphsLoaded, charset.size(), fontPath.filename());

		double emSize = 40.0;

		msdf_atlas::TightAtlasPacker atlasPacker;
		//atlasPacker.setDimensionsConstraint()
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setMiterLimit(1.0);
		atlasPacker.setPadding(0);
		atlasPacker.setScale(emSize);
		int remaing = atlasPacker.pack(m_data->glyphs.data(), (int)m_data->glyphs.size());
		DD_CORE_ASSERT(remaing == 0);

		int width, height;
		atlasPacker.getDimensions(width, height);
		emSize = atlasPacker.getScale();

		// if MSDF || MTSDF

		// Temporary
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define DEFAULT_ANGLE_THRESHOLD 3.0
#define THREAD_COUNT 8

		uint64_t coloringSeed = 0;

		bool expensiveColoring = false;
		if (expensiveColoring) {
			msdf_atlas::Workload([&glyphs = m_data->glyphs, &coloringSeed](int i, int threadNo) -> bool {
				unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
				glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				return true;
				}, m_data->glyphs.size()).finish(THREAD_COUNT);
		}
		else {
			unsigned long long glyphSeed = coloringSeed;
			for (msdf_atlas::GlyphGeometry& glyph : m_data->glyphs) {
				glyphSeed *= LCG_MULTIPLIER;
				glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
			}
		}

		m_atlasTexture = create_and_cache_atlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Test", (float)emSize, m_data->glyphs, m_data->fontGeometry, width, height);

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
	}

	Font::~Font()
	{
		delete m_data;
	}

	Shr_ptr<Font> Font::getDefault()
	{
		// NOTE: lazy loaded might change later
		static Shr_ptr<Font> defaultFont;
		if (!defaultFont)
		{
			auto [path, check] = utils::get_core_file_location("resources\\fonts\\Noto_Sans\\NotoSans-Medium.ttf");
			DD_CORE_ASSERT(check, "Cant find default font");
			defaultFont = create_shr_ptr<Font>(path);
		}

		return defaultFont;
	}

}