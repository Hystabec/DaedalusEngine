#include "editorpch.h"
#include "contentBrowserPanel.h"

#include <imgui.h> 
#include <filesystem>

namespace daedalus::editor
{

	static const std::filesystem::path s_assetPath = "assets";

	// TO DO: This will need to be changed if the scene extention is ever changed
	static const std::string s_sceneExtention = ".Daedalus";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_currentDirectory(s_assetPath)
	{
		m_directoryIcon = graphics::Texture2D::create("resources/icons/contentBrowser/directoryIcon.png");
		m_fileIcon = graphics::Texture2D::create("resources/icons/contentBrowser/fileIcon.png");
	}

	void ContentBrowserPanel::onImGuiRender()
	{
		static std::string currentDirPath = s_assetPath.string();
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;

		ImGui::Begin("Content Browser");

		// TO DO: make this more visually apearling, currently function in generally the way i want
		ImGui::Text("Path:");
		ImGui::SameLine();
		std::string currentSubStr = "";
		for (uint32_t i = 0; i < currentDirPath.size(); i++)
		{
			char& c = currentDirPath[i];
			if (c == '/' || c == '\\')
			{
				if (ImGui::Button(currentSubStr.c_str()))
				{
					m_currentDirectory = std::filesystem::path(currentDirPath.substr(0, currentDirPath.find(currentSubStr) + currentSubStr.size()));
					currentDirPath = m_currentDirectory.string();
					ImGui::SameLine();	// this is here so there isnt a little jump when clicking a button
					break;
				}
				ImGui::SameLine();
				ImGui::Text("/");
				ImGui::SameLine();

				currentSubStr = "";
			}
			else
				currentSubStr += c;
		}
		if (currentSubStr != "")
		{
			//ImGui::Text("%s", currentSubStr.c_str());
			ImGui::Button(currentSubStr.c_str());
		}
		//ImGui::TextWrapped("Path: %s", clickedFilePath.c_str());

		ImGui::Separator();

		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);

		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryElement : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryElement.path();
			std::string filenameStr = path.filename().string();
			
			ImGui::PushID(filenameStr.c_str());

			auto& elementIcon = directoryElement.is_directory() ? m_directoryIcon : m_fileIcon;

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 });
			ImGui::ImageButton("##ImageButton", (ImTextureID)elementIcon->getRendererID(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});

			if(path.extension().string() == s_sceneExtention)
			{
				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = path.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
					ImGui::Image((ImTextureID)elementIcon->getRendererID(), ImVec2{ 64, 64 }, { 0, 1 }, { 1, 0 });
					ImGui::EndDragDropSource();
				}
			}

			ImGui::PopStyleColor();

			bool windowIsHovered = ImGui::IsItemHovered();
			if (windowIsHovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryElement.is_directory())
				{
					m_currentDirectory /= path.filename();
					currentDirPath = m_currentDirectory.string();
				}
			}

			ImGui::TextWrapped(filenameStr.c_str());
			ImGui::PopID();

			ImGui::NextColumn();
		}

		ImGui::Columns(1);
		
		/*if (m_currentDirectory != s_assetPath)
		{
			if (ImGui::Button("<-"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
				currentDirPath = m_currentDirectory.string();
			}
		}*/

#if 0
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 64, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);
#endif

		ImGui::End();
	}

}