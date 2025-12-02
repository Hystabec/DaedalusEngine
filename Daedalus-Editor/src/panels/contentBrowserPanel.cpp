#include "editorpch.h"
#include "contentBrowserPanel.h"

#include "project/Project.h"
#include "asset/textureImporter.h"

#include <imgui.h> 
#include <filesystem>
#include <regex>

// TO DO: When testing is finished remove macros and statements

// The current font scaling is a little hacky, so if i want different size fonts i should
// use the actual system, current setup is to get a feel for the different sizes
#define ENABLE_FONT_SCALING 0
#define ENABLE_SIZE_SLIDERS 0

namespace daedalus::editor
{
	ContentBrowserPanel::ContentBrowserPanel()
	{
		m_directoryIcon = TextureImporter::loadTexture2D("resources/icons/contentBrowser/directoryIcon.png");
		m_fileIcon = TextureImporter::loadTexture2D("resources/icons/contentBrowser/fileIcon.png");
		m_scriptFileIcon = TextureImporter::loadTexture2D("resources/icons/contentBrowser/scriptFileIcon.png");
		m_sceneFileIcon = TextureImporter::loadTexture2D("resources/icons/contentBrowser/sceneFileIcon.png");
	}

	void ContentBrowserPanel::onImGuiRender()
	{
		static float padding = 16.0f;
		static float thumbnailSize = 96.0f;
#if ENABLE_FONT_SCALING
		static float defaultFontScale = thumbnailSize;
		float fontScale = thumbnailSize / defaultFontScale;
#endif
		ImGui::Begin("Content Browser");

		// No valid project
		if (m_projectAssetDirectory.empty())
		{
			ImGui::End();
			return;
		}

		// TO DO: make this more visually apearling, currently function in generally the way i want
		{
			std::string currentDirPath = m_currentDirectory.string();
			ImGui::Text("Path:");
			ImGui::SameLine();

			if (ImGui::Button("assets"))
			{
				m_currentDirectory = "";
				currentDirPath = ""; // this is made empty so the for loop doesnt run
			}
			if (!currentDirPath.empty())
			{
				ImGui::SameLine();
				ImGui::Text("/");
				ImGui::SameLine();
			}

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
				ImGui::Button(currentSubStr.c_str());
			}
		}

		ImGui::Separator();

		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);

		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);
#if ENABLE_FONT_SCALING
		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault->FontSize /= fontScale;
#endif

		for (auto& directoryElement : std::filesystem::directory_iterator(m_projectAssetDirectory / m_currentDirectory))
		{
			const auto& path = directoryElement.path();
			std::string filenameStr = path.filename().string();

			if (!m_filtedFilePathMap.contains(path))
				filterDirectory(m_projectAssetDirectory / m_currentDirectory);

			if (!m_filtedFilePathMap[path])
				continue;

			ImGui::PushID(filenameStr.c_str());

			Shr_ptr<graphics::Texture2D> elementIcon;
			if (directoryElement.is_directory())
				elementIcon = m_directoryIcon;
			else
			{
				if (path.extension() == ".cs")
					elementIcon = m_scriptFileIcon;
				else if (path.extension() == ".ddscene")
					elementIcon = m_sceneFileIcon;
				else
					elementIcon = m_fileIcon;
			}

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0, 0, 0, 0 });
			ImGui::ImageButton("##ImageButton", (ImTextureID)elementIcon->getRendererID(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = path.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::Image((ImTextureID)elementIcon->getRendererID(), ImVec2{ 64, 64 }, { 0, 1 }, { 1, 0 });
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			bool windowIsHovered = ImGui::IsItemHovered();
			if (windowIsHovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryElement.is_directory())
				{
					m_currentDirectory /= path.filename();
					//currentDirPath = m_currentDirectory.string();
				}
			}

			ImGui::TextWrapped(filenameStr.c_str());
			ImGui::PopID();

			ImGui::NextColumn();
		}

#if ENABLE_FONT_SCALING
		io.FontDefault->FontSize *= fontScale;
#endif
		ImGui::Columns(1);

#if ENABLE_SIZE_SLIDERS
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 64, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);
#endif

		ImGui::End();
	}

	void ContentBrowserPanel::filterDirectory(const std::filesystem::path& directoryPath)
	{
		static const auto& lambaFilters = [](const std::vector<std::string>& filters)
			{
				std::string parsedFilters = "";

				for (const auto& str : filters)
				{
					if (str.empty())
						continue;

					if (!parsedFilters.empty())
						parsedFilters.push_back('|');

					for (char c : str)
					{
						switch (c)
						{
						// special regex chars
						case '.': case '?': case '*': case '+': case '`':
						case '^': case '$':
						case '[': case ']': case '(': case ')':
							parsedFilters.push_back('\\');
							break;
						case '\\': case '/':
							parsedFilters += R"((?:\\|\/))";
							continue;
						}
						parsedFilters.push_back(c);
					}
				}

				return parsedFilters;
			};

		//R"((?:(?:(?=.*?\\).*?\\|^)(?=(script-bin|\.vs))\1($|(?=\\)\\.*))|(.*?(\.lua|\.csproj|\.sln|\.bat)))"
		static std::regex reg(R"((?:(?:(?=.*?(?:\\|\/)).*?(?:\\|\/)|^)(?=()" + lambaFilters(m_directoryFilters) + R"())\1($|(?=\\|\/)(?:\\|\/).*))|(.*?()" + lambaFilters(m_fileFilters) + R"()))", std::regex_constants::icase);

		for (auto& directoryElement : std::filesystem::directory_iterator(directoryPath))
		{
			if (m_filtedFilePathMap.contains(directoryElement.path()))
				continue;

			m_filtedFilePathMap.insert(std::pair<std::filesystem::path,bool>(directoryElement.path(), !std::regex_match(directoryElement.path().string(), reg)));
		}
	}

}