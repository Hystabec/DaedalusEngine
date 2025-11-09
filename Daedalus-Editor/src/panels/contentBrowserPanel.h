#pragma once

#include "graphics/rendering/texture.h"

#include <filesystem>

namespace daedalus::editor
{
	// TO DO: Consider using regex expressions to filter files and directories
	// check how fast regex is as it will be doing it many times each update

	/// @brief Any files in the set will be filtered out
	static const std::unordered_set<std::string> contentBrowserFileFilters = { "premake5.lua", "Sandbox.csproj", "Sandbox.sln", "win-GenerateProject-VS22.bat" };
	/// @brief Any directories in the set will be filtered out
	static const std::unordered_set<std::string> contentBrowserDirectoryFilters = { ".vs", "script-bin" };

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