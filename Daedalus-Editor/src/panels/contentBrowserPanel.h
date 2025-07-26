#pragma once

#include <filesystem>

namespace daedalus::editor
{

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void onImGuiRender();

	private:
		std::filesystem::path m_currentDirectory;
	};

}