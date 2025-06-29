#pragma once

namespace daedalus::editor
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Shr_ptr<scene::Scene>& scene);

		void setContext(const Shr_ptr<scene::Scene>& scene);

		void onImGuiRender();
	private:
		void drawEntityNode(scene::Entity entity);
		void drawComponents(scene::Entity entity);

	private:
		Shr_ptr<scene::Scene> m_sceneContext;
		scene::Entity m_selectionContext;
	};

}