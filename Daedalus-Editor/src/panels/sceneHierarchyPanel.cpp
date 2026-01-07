#include "editorpch.h"
#include "sceneHierarchyPanel.h"

#include "scripting/scriptEngine.h"
#include "ui/ui.h"
#include "asset/assetManager.h"
#include "asset/editorAssetManager.h"
#include "asset/importers/textureImporter.h"

//#include "scene/components.h"
#include <imgui.h> 
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

namespace daedalus::editor
{

	SceneHierarchyPanel::SceneHierarchyPanel(const IntrusivePtr<scene::Scene>& scene)
	{
		setContext(scene);
	}

	void SceneHierarchyPanel::setContext(const IntrusivePtr<scene::Scene>& scene)
	{
		m_sceneContext = scene;
		m_selectionContext = {};
	}

	void SceneHierarchyPanel::onImGuiRender()
	{
		using namespace scene;

		ImGui::Begin("Scene Hierarchy");

		for (auto entityID : m_sceneContext->m_registry.view<entt::entity>())
		{
			Entity entity{ entityID, m_sceneContext.get() };
			drawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_selectionContext = {};

		// Open context menu on black space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_sceneContext->createEntity("Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		// TO DO: move into own class
		ImGui::Begin("Properties");

		if (m_selectionContext)
		{
			drawComponents(m_selectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::setSelectedEntity(scene::Entity entity)
	{
		m_selectionContext = entity;
	}

	template<typename T>
	void SceneHierarchyPanel::displayAddComponentEntry(const std::string& entryName)
	{
		if (!m_selectionContext.hasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_selectionContext.addComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void SceneHierarchyPanel::drawEntityNode(scene::Entity entity)
	{
		const auto& name = entity.getName();
		
		const ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_sceneContext->destroyEntity(entity);
			if (m_selectionContext == entity)
				m_selectionContext = {};
		}
	}

	/// @brief This will draw the data for a specified component using a lambda function
	/// @brief Taking entity as a ref for both this func and the lambda as i didnt want to make 3 different copies,
	/// even though it is a single uint32_t
	template<typename T, typename componentFunction>
	static void draw_component(const std::string& label, componentFunction func, scene::Entity& entity, bool componentIsRemovable = true)
	{
		if (!entity.hasComponent<T>())
			return;

		static const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap 
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

		ImVec2 contentRegionAvalible = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		//ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.c_str());
		ImGui::PopStyleVar();

		bool removeComponent = false;
		if (componentIsRemovable)
		{
			ImGui::SameLine(contentRegionAvalible.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}
		}

		if (open)
		{
			func(entity.getComponent<T>());
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.removeComponent<T>();
	}

	void SceneHierarchyPanel::drawComponents(scene::Entity entity)
	{
		// This check is kind of pointless as all entities should have a Tag
		if (entity.hasComponent<scene::IDComponent>())
		{
			auto& name = entity.getComponent<scene::IDComponent>().name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), name.c_str());
			if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			{
				name = std::string(buffer);
			}
			ImGui::SetItemTooltip("ID: %llu", entity.getUUID());
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			// this could be like copy_component in scene.cpp
			displayAddComponentEntry<scene::ScriptComponent>("Script");
			displayAddComponentEntry<scene::CameraComponent>("Camera");
			displayAddComponentEntry<scene::SpriteRendererComponent>("Sprite Renderer");
			displayAddComponentEntry<scene::CircleRendererComponent>("Circle Renderer");
			displayAddComponentEntry<scene::TextComponent>("Text");
			displayAddComponentEntry<scene::Rigidbody2DComponent>("Rigidbody 2D");
			displayAddComponentEntry<scene::BoxCollider2DComponent>("Box Collider 2D");
			displayAddComponentEntry<scene::CircleCollider2DComponent>("Circle Collider 2D");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		draw_component<scene::TagComponent>("Tag",
			[](scene::TagComponent& tc)
			{
				auto& tag = tc.tag;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tag.c_str());
				if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}

			}, entity, false);

		draw_component<scene::TransformComponent>("Transform",
			[](scene::TransformComponent& tc)
			{
				ui::draw_vec3_control("Position", tc.position);

				maths::Vec3 rotation = maths::radians_to_degrees(tc.rotation);
				if(ui::draw_vec3_control("Rotation", rotation, 1.0f))
					tc.rotation = maths::degrees_to_radians(rotation);

				ui::draw_vec3_control("Scale", tc.scale, 0.1f, 1.0f);
			}, entity, false);

		draw_component<scene::ScriptComponent>("Script",
			[entity, scene = m_sceneContext](scene::ScriptComponent& sc)
			{
				bool scriptClassExists = scripting::ScriptEngine::entityClassExists(sc.className);

				static char buffer[64];
				strcpy_s(buffer, sc.className.c_str());

				ui::ScopedStyleColor scopedTextColour(ImGuiCol_Text, ImVec4(0.9f, 0.1f, 0.1f, 1.0f), !scriptClassExists);

				if (ImGui::InputText("Class", buffer, sizeof(buffer)))
				{
					sc.className = buffer;
					return;
				}

				// Fields
				bool isSceneRunning = scene->isRunning();
				if (isSceneRunning)
				{
					IntrusivePtr<scripting::ScriptInstance> scriptInstance = scripting::ScriptEngine::getEntityScriptInstance(entity.getUUID());
					if (scriptInstance)
					{
						const auto& fields = scriptInstance->getScriptClass()->getFields();

						for (const auto& [name, field] : fields)
						{
							if (field.type == scripting::ScriptFieldType::Float)
							{
								float data = scriptInstance->getFieldValue<float>(name);
								if (ImGui::DragFloat(name.c_str(), &data))
								{
									scriptInstance->setFieldValue(name, data);
								}
							}
						}
					}
				}
				else
				{
					if (scriptClassExists)
					{
						auto entityClass = scripting::ScriptEngine::getEntityClass(sc.className);
						auto& fields = entityClass->getFields();

						auto& classFields = scripting::ScriptEngine::getEntityScriptFields(entity.getUUID());

						for (const auto& [name, field] : fields)
						{
							// Field set in editor
							if (classFields.find(name) != classFields.end())
							{
								scripting::ScriptFieldInstance& scriptField = classFields.at(name);

								if (field.type == scripting::ScriptFieldType::Float)
								{
									float data = scriptField.getFieldValue<float>();
									if (ImGui::DragFloat(name.c_str(), &data))
									{
										scriptField.setFieldValue(data);
									}
								}
							}
							else
							{
								if (field.type == scripting::ScriptFieldType::Float)
								{
									// TO DO: load the default value for the C# field
									float data = 0.0f;
									if (ImGui::DragFloat(name.c_str(), &data))
									{
										auto& fieldInstance = classFields[name];
										fieldInstance.field = field;
										fieldInstance.setFieldValue(data);
									}
								}
							}
						}
					}
				}

			}, entity);
		
		draw_component<scene::CameraComponent>("Camera",
			[](scene::CameraComponent& cameraComp)
			{
				auto& camera = cameraComp.camera;

				ImGui::Checkbox("Primary", &cameraComp.primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.setProjectionType((scene::SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (cameraComp.camera.getProjectionType() == scene::SceneCamera::ProjectionType::Perspective)
				{
					float perspecVertivalFov = maths::radians_to_degrees(camera.getPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspecVertivalFov))
						camera.setPerspectiveVerticalFOV(maths::degrees_to_radians(perspecVertivalFov));

					float perspecNear = camera.getPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspecNear))
						camera.setPerspectiveNearClip(perspecNear);

					float perspecFar = camera.getPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspecFar))
						camera.setPerspectiveFarClip(perspecFar);
				}

				if (cameraComp.camera.getProjectionType() == scene::SceneCamera::ProjectionType::Othographic)
				{
					float orthoSize = camera.getOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.setOrthographicSize(orthoSize);

					float orthoNear = camera.getOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.setOrthographicNearClip(orthoNear);

					float orthoFar = camera.getOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.setOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed aspect ratio", &cameraComp.fixedAspectRatio);
				}
			}, entity);

		draw_component<scene::SpriteRendererComponent>("Sprite Renderer",
			[](scene::SpriteRendererComponent& spriteRenderer)
			{
				ImGui::ColorEdit4("Colour", spriteRenderer.colour);
				// Texture
				std::string label = "None";
				const AssetHandle& textureHandle = spriteRenderer.material.texture;
				bool isTextureValid = false;
				if (textureHandle != 0)
				{
					if (AssetManager::isAssetHandleValid(textureHandle) && AssetManager::getAssetType(textureHandle) == AssetType::Texture2D)
					{
						const auto& metadata = Project::getActive()->getEditorAssetManager()->getMetadata(textureHandle);
						label = metadata.filepath.filename().string();
						isTextureValid = true;
					}
					else
						label = "Invalid";
				}
				
				ImVec2 buttonLabelSize = ImGui::CalcTextSize(label.c_str());
				buttonLabelSize.x += 20.0f;
				float buttonLabelWidth = maths::max(100.0f, buttonLabelSize.x);

				ImGui::Button(label.c_str(), ImVec2(buttonLabelWidth, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						std::filesystem::path path = (const wchar_t*)payload->Data;
						if (utils::FileSystem::fileExtensionToLower(path.extension()) == ".png")
						{
							auto relativePath = std::filesystem::relative(path, Project::getActiveAssetDirectory());
							AssetHandle handle = Project::getActive()->getEditorAssetManager()->importAsset(relativePath);
							if (handle)
								spriteRenderer.material.texture = handle;
						}
					}
					ImGui::EndDragDropTarget();
				}

				if (isTextureValid)
				{
					ImGui::SameLine();
					ImVec2 xLabelSize = ImGui::CalcTextSize("X");
					float buttonSize = xLabelSize.y + ImGui::GetStyle().FramePadding.y * 2.0f;
					if(ImGui::Button("X", ImVec2(buttonSize, buttonSize)))
					{
						spriteRenderer.material.texture = 0;
					}
				}
				ImGui::SameLine();
				ImGui::Text("Texture");

				ImGui::DragFloat("Tiling Factor", &spriteRenderer.material.tilingFactor, 0.1f, 0.0f);

			}, entity);

		draw_component<scene::CircleRendererComponent>("Circle Renderer",
			[](scene::CircleRendererComponent& cRenderer)
			{
				ImGui::ColorEdit4("Colour", cRenderer.colour);
				ImGui::DragFloat("Thickness", &cRenderer.thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &cRenderer.fade, 0.00025f, 0.0f, 5.0f);

			}, entity);

		draw_component<scene::TextComponent>("Text",
			[](scene::TextComponent& textComp)
			{
				ImGui::InputTextMultiline("Text String", &textComp.text);
				ImGui::ColorEdit4("Colour", textComp.colour);
				ImGui::DragFloat("Kerning", &textComp.kerning, 0.05f);
				ImGui::DragFloat("Line spacing", &textComp.lineSpacing, 0.05f);
			}, entity);

		draw_component<scene::Rigidbody2DComponent>("Rigidbody 2D",
			[](scene::Rigidbody2DComponent& rb2d)
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentbodyTypeString = bodyTypeStrings[(int)rb2d.type];
				if (ImGui::BeginCombo("Body Type", currentbodyTypeString))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentbodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentbodyTypeString = bodyTypeStrings[i];
							rb2d.type = (scene::Rigidbody2DComponent::BodyType)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &rb2d.fixedRotation);
				ImGui::DragFloat("Desity", &rb2d.desity, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &rb2d.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &rb2d.restitution, 0.01f, 0.0f, 1.0f);

			}, entity);

		draw_component<scene::BoxCollider2DComponent>("Box Collider 2D",
			[](scene::BoxCollider2DComponent& bc2d)
			{
				ImGui::DragFloat2("Offset", (float*)bc2d.offset, 0.1f);
				ImGui::DragFloat2("Size", (float*)bc2d.size, 0.1f);

			}, entity);

		draw_component<scene::CircleCollider2DComponent>("Circle Collider 2D",
			[](scene::CircleCollider2DComponent& bc2d)
			{
				ImGui::DragFloat2("Offset", (float*)bc2d.offset, 0.1f);
				ImGui::DragFloat("Radius", &bc2d.radius, 0.1f, 0.0f);

			}, entity);
	}

}