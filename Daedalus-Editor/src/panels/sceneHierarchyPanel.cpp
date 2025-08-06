#include "editorpch.h"
#include "sceneHierarchyPanel.h"

//#include "scene/components.h"
#include <imgui.h> 
#include <imgui_internal.h>

namespace daedalus::editor
{

	SceneHierarchyPanel::SceneHierarchyPanel(const Shr_ptr<scene::Scene>& scene)
	{
		setContext(scene);
	}

	void SceneHierarchyPanel::setContext(const Shr_ptr<scene::Scene>& scene)
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

	void SceneHierarchyPanel::drawEntityNode(scene::Entity entity)
	{
		auto& tag = entity.getComponent<scene::TagComponent>().tag;
		
		static const ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
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
	template<typename T>
	static void draw_component(const std::string& label, void(*func)(T&), scene::Entity& entity, bool componentIsRemovable = true)
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

	// TO DO: Move into UI libary
	static bool draw_vec3_control(const std::string& label, maths::Vec3& values, float dragSensitivity = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		bool itemChange = false;

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
			itemChange = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, dragSensitivity, 0.0f, 0.0f, "%.1f"))
			itemChange = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.7f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.7f, 0.1f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
			itemChange = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, dragSensitivity, 0.0f, 0.0f, "%.1f"))
			itemChange = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.7f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.7f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
			itemChange = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &values.z, dragSensitivity, 0.0f, 0.0f, "%.1f"))
			itemChange = true;
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return itemChange;
	}

	void SceneHierarchyPanel::drawComponents(scene::Entity entity)
	{
		// This check is kind of pointless as all entities should have a Tag
		if (entity.hasComponent<scene::TagComponent>())
		{
			auto& tag = entity.getComponent<scene::TagComponent>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
			ImGui::SetItemTooltip("ID: %llu", entity.getUUID());
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (!m_selectionContext.hasComponent<scene::CameraComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_selectionContext.addComponent<scene::CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_selectionContext.hasComponent<scene::SpriteRendererComponent>())
			{
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_selectionContext.addComponent<scene::SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_selectionContext.hasComponent<scene::CircleRendererComponent>())
			{
				if (ImGui::MenuItem("Circle Renderer"))
				{
					m_selectionContext.addComponent<scene::CircleRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_selectionContext.hasComponent<scene::Rigidbody2DComponent>())
			{
				if (ImGui::MenuItem("Rigidbody 2D"))
				{
					m_selectionContext.addComponent<scene::Rigidbody2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_selectionContext.hasComponent<scene::BoxCollider2DComponent>())
			{
				if (ImGui::MenuItem("Box Collider 2D"))
				{
					m_selectionContext.addComponent<scene::BoxCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_selectionContext.hasComponent<scene::CircleCollider2DComponent>())
			{
				if (ImGui::MenuItem("Circle Collider 2D"))
				{
					m_selectionContext.addComponent<scene::CircleCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		draw_component<scene::TransformComponent>("Transform",
			[](scene::TransformComponent& tc)
			{
				draw_vec3_control("Position", tc.position);

				maths::Vec3 rotation = maths::radians_to_degrees(tc.rotation);
				if(draw_vec3_control("Rotation", rotation, 1.0f))
					tc.rotation = maths::degrees_to_radians(rotation);

				draw_vec3_control("Scale", tc.scale, 0.1f, 1.0f);
			}, entity, false);
		
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
				ImGui::Button("Texture", ImVec2(100.0f, 0.0f));

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						std::filesystem::path path = (const wchar_t*)payload->Data;
						if (path.extension().string() == ".png")
							spriteRenderer.material.texture = graphics::Texture2D::create(path.string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("Tiling Factor", &spriteRenderer.material.tilingFactor, 0.1f, 0.0f);

			}, entity);

		draw_component<scene::CircleRendererComponent>("Circle Renderer",
			[](scene::CircleRendererComponent& cRenderer)
			{
				ImGui::ColorEdit4("Colour", cRenderer.colour);
				ImGui::DragFloat("Thickness", &cRenderer.thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &cRenderer.fade, 0.00025f, 0.0f, 5.0f);

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

			}, entity);

		draw_component<scene::BoxCollider2DComponent>("Box Collider 2D",
			[](scene::BoxCollider2DComponent& bc2d)
			{
				ImGui::DragFloat2("Offset", (float*)bc2d.offset, 0.1f);
				ImGui::DragFloat2("Size", (float*)bc2d.size, 0.1f);
				ImGui::DragFloat("Desity", &bc2d.desity, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &bc2d.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &bc2d.restitution, 0.01f, 0.0f, 1.0f);
				//ImGui::DragFloat("Restitution Threshold", &bc2d.restitutionThreshold);

			}, entity);

		draw_component<scene::CircleCollider2DComponent>("Circle Collider 2D",
			[](scene::CircleCollider2DComponent& bc2d)
			{
				ImGui::DragFloat2("Offset", (float*)bc2d.offset, 0.1f);
				ImGui::DragFloat("Radius", &bc2d.radius, 0.1f, 0.0f);
				ImGui::DragFloat("Desity", &bc2d.desity, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &bc2d.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &bc2d.restitution, 0.01f, 0.0f, 1.0f);

			}, entity);
	}

}