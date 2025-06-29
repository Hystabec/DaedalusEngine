#include "editorpch.h"
#include "sceneHierarchyPanel.h"

#include <imgui.h> 
//#include "scene/components.h"

namespace daedalus::editor
{

	SceneHierarchyPanel::SceneHierarchyPanel(const Shr_ptr<scene::Scene>& scene)
	{
		setContext(scene);
	}

	void SceneHierarchyPanel::setContext(const Shr_ptr<scene::Scene>& scene)
	{
		m_sceneContext = scene;
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

		ImGui::End();

		// TODO: move into own class
		ImGui::Begin("Properties");

		if (m_selectionContext)
			drawComponents(m_selectionContext);

		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(scene::Entity entity)
	{
		auto& tag = entity.getComponent<scene::TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::drawComponents(scene::Entity entity)
	{
		if (entity.hasComponent<scene::TagComponent>())
		{
			auto& tag = entity.getComponent<scene::TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.hasComponent<scene::TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(scene::TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.getComponent<scene::TransformComponent>().Transform;
				ImGui::DragFloat3("Position", transform.columns[3], 0.25f);

				ImGui::TreePop();
			}
		}

		if (entity.hasComponent<scene::CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(scene::CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComp = entity.getComponent<scene::CameraComponent>();
				auto& camera = cameraComp.Camera;

				ImGui::Checkbox("Primary", &cameraComp.Primary);

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

				if (cameraComp.Camera.getProjectionType() == scene::SceneCamera::ProjectionType::Perspective)
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

				if (cameraComp.Camera.getProjectionType() == scene::SceneCamera::ProjectionType::Othographic)
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

					ImGui::Checkbox("Fixed aspect ratio", &cameraComp.FixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}
	}

}