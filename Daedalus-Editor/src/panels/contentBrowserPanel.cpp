#include "editorpch.h"
#include "contentBrowserPanel.h"

#include <imgui.h> 
#include <filesystem>

namespace daedalus::editor
{

	static const std::filesystem::path s_assetPath = "resources";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_currentDirectory(s_assetPath)
	{
	}

	void ContentBrowserPanel::onImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_currentDirectory != s_assetPath)
		{
			if (ImGui::Button("<-"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}

		for (auto& directoryElement : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryElement.path();
			//auto relativePath = std::filesystem::relative(path, s_assetPath);
			std::string filenameStr = path.filename().string();

			if (directoryElement.is_directory())
			{
				if (ImGui::Button(filenameStr.c_str()))
				{
					m_currentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filenameStr.c_str()))
				{

				}
			}
		}

		ImGui::End();
	}

}