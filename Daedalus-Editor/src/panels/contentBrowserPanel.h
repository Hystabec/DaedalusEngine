#pragma once

#include <filesystem>

#include "graphics/rendering/texture.h"

namespace daedalus::editor
{

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void onImGuiRender();

	private:
		std::filesystem::path m_currentDirectory;

		Shr_ptr<graphics::Texture2D> m_directoryIcon;
		Shr_ptr<graphics::Texture2D> m_fileIcon;
	};

}