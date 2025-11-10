#pragma once

#include "graphics/rendering/texture.h"

#include <filesystem>

namespace daedalus::editor
{
	// TO DO: Consider using regex expressions to filter files and directories
	// check how fast regex is as it will be doing it many times each update
	// look into using glob, which is used for thing like the .gitignore

	/// @brief Any files in the set will be filtered out
	static const std::unordered_set<std::string> contentBrowserFileFilters = 
	{ 
		".vs", "script-bin",
		".lua", ".csproj", ".sln", ".bat"
	};

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void onImGuiRender();

		void setProjectAssetDirectory(const std::filesystem::path& projectAssetPath) { m_projectAssetDirectory = projectAssetPath; m_currentDirectory = ""; }

	private:
		std::filesystem::path m_projectAssetDirectory;
		std::filesystem::path m_currentDirectory;

		Shr_ptr<graphics::Texture2D> m_directoryIcon;
		Shr_ptr<graphics::Texture2D> m_fileIcon;
	};

}