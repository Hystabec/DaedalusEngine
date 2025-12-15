#pragma once

#include "graphics/rendering/texture.h"

#include <filesystem>

namespace daedalus::editor
{
	// NOTE: To filter directories/files a regex is used, this might not be the best way
	// but i wanted to use more regexs as I havent used many before

	// NOTE: could store the filters in a file or something and load them in to make them easier to use
	// currently the filtes just sit inside of ContentBrowserPanel

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void onImGuiRender();

		void setProjectAssetDirectory(const std::filesystem::path& projectAssetPath) 
		{
			m_projectAssetDirectory = projectAssetPath;
			m_currentDirectory = "";
			m_filtedFilePathMap.clear(); // clear the map so it doesnt contain lots of (potentially) useless file paths
		}

	private:
		void filterDirectory(const std::filesystem::path& directoryPath);
		void refreshRegistyAssets();

	private:
		const std::vector<std::string> m_directoryFilters = { ".vs", "script-bin" };
		const std::vector<std::string> m_fileFilters =		{ ".lua", ".csproj", ".sln", ".bat", ".ddreg" };
		std::unordered_map<std::filesystem::path, bool> m_filtedFilePathMap;

		std::filesystem::path m_projectAssetDirectory;
		std::filesystem::path m_currentDirectory;

		Shr_ptr<graphics::Texture2D> m_directoryIcon, m_fileIcon, m_scriptFileIcon, m_sceneFileIcon;

		// AssetRegisty shows all the assets in the asset registy, items that have been imported
		// FileSytem will show what is on disk regardless of what it is, so items can be imported
		// NOTE: With the fileSystem mode I could have a little conext symbol that shows if its imported or not
		enum class Mode
		{
			AssetRegisty = 0, FileSystem = 1
		};

		Mode m_mode = Mode::AssetRegisty;
		std::set<std::filesystem::path> m_registyAssets;
	};

}