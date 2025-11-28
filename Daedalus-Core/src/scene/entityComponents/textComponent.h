#pragma once

#include "../../graphics/rendering/font.h"
#include "../../core.h"

#include <string>

namespace daedalus::scene {

	struct TextComponent
	{
		std::string text;
		daedalus::Shr_ptr<graphics::Font> fontAsset = graphics::Font::getDefault();
		maths::Vec4 colour{ 0.0f, 0.0f, 0.0f, 1.0f };
		float kerning = 0.0f;
		float lineSpacing = 0.0f;

		TextComponent() = default;
		TextComponent(const TextComponent&) = default;
	};

}