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

}